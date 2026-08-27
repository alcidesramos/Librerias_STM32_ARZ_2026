#!/usr/bin/env python3
"""
N6 FSBL Flash Tool
------------------
GUI sencilla (Tkinter, sin dependencias externas) para:

  1) "Generar trusted.bin": compila el proyecto FSBL y ejecuta los pasos
     POST_BUILD ya configurados en FSBL/CMakeLists.txt (objcopy + STM32
     Signing Tool), produciendo <Proyecto>_trusted.bin listo para flashear.

  2) "Programar en flash externa": llama a STM32_Programmer_CLI para
     conectarse por ST-LINK (SWD), seleccionar el external loader y
     escribir el binario firmado en la direccion 0x70000000 de la flash
     externa, verificar y resetear el target. Todo con un solo click,
     sin abrir la GUI de STM32CubeProgrammer.

Uso:
    python tools/flash_tool.py

La carpeta del proyecto se detecta automaticamente (carpeta padre de
"tools/"), pero se puede cambiar con el boton "Cambiar...".
"""

import glob
import json
import os
import queue
import shutil
import subprocess
import sys
import threading
import tkinter as tk
from tkinter import filedialog, messagebox, ttk

# --------------------------------------------------------------------------- #
# Configuracion / rutas
# --------------------------------------------------------------------------- #

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
CONFIG_PATH = os.path.join(SCRIPT_DIR, "flash_tool_config.json")

FSBL_FLASH_ADDRESS = "0x70000000"
APPNS_FLASH_ADDRESS = "0x70100000"
EXTERNAL_FLASH_BASE = "0x80000000"
SIGNING_TOOL_NAME = "STM32_SigningTool_CLI.exe"
OBJCOPY_CANDIDATES = (
    "arm-none-eabi-objcopy",
    "arm-none-eabi-objcopy.exe",
    "objcopy",
    "objcopy.exe",
)

CUBEPROGRAMMER_BIN_CANDIDATES = [
    r"C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin",
    r"C:\Program Files (x86)\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin",
]


def load_config():
    if os.path.isfile(CONFIG_PATH):
        try:
            with open(CONFIG_PATH, "r", encoding="utf-8") as f:
                return json.load(f)
        except (OSError, json.JSONDecodeError):
            pass
    return {}


def save_config(cfg):
    try:
        with open(CONFIG_PATH, "w", encoding="utf-8") as f:
            json.dump(cfg, f, indent=2)
    except OSError:
        pass


def find_cubeprogrammer_bin():
    """Busca la carpeta bin/ de STM32CubeProgrammer (CLI + signing tool + loaders)."""
    for candidate in CUBEPROGRAMMER_BIN_CANDIDATES:
        if os.path.isdir(candidate):
            return candidate
    # Como ultimo recurso, ver si esta en PATH
    for exe in ("STM32_Programmer_CLI.exe", "STM32_Programmer_CLI"):
        for path_dir in os.environ.get("PATH", "").split(os.pathsep):
            if os.path.isfile(os.path.join(path_dir, exe)):
                return path_dir
    return None


def find_objcopy():
    for name in OBJCOPY_CANDIDATES:
        found = shutil.which(name)
        if found:
            return found
    return None


def default_project_dir():
    # tools/ vive en la raiz del proyecto -> la carpeta padre es la raiz
    return os.path.abspath(os.path.join(SCRIPT_DIR, ".."))


# --------------------------------------------------------------------------- #
# GUI
# --------------------------------------------------------------------------- #


class FlashToolApp:
    def __init__(self, root):
        self.root = root
        self.root.title("N6 FSBL - Flash Tool_ARZ")
        self.root.geometry("1000x560")

        self.cfg = load_config()
        self.project_dir = tk.StringVar(
            value=self.cfg.get("project_dir") or default_project_dir()
        )
        self.cubeprog_bin = self.cfg.get("cubeprog_bin") or find_cubeprogrammer_bin() or ""
        self.loader_var = tk.StringVar()
        self.connect_mode_var = tk.StringVar(value=self.cfg.get("connect_mode", "NORMAL"))
        self.freq_var = tk.StringVar(value=self.cfg.get("freq", "8000"))

        self.log_queue = queue.Queue()
        self.busy = False

        self._build_ui()
        self._refresh_loaders()
        self._poll_log_queue()

    # ---------------------------------------------------------------- UI --

    def _build_ui(self):
        pad = {"padx": 8, "pady": 6}

        top = ttk.Frame(self.root)
        top.pack(fill="x", **pad)

        ttk.Label(top, text="Carpeta del proyecto:").grid(row=0, column=0, sticky="w")
        self.project_entry = ttk.Entry(top, textvariable=self.project_dir, width=80)
        self.project_entry.grid(row=0, column=1, sticky="we", padx=4)
        ttk.Button(top, text="Cambiar...", command=self._choose_project_dir).grid(
            row=0, column=2
        )
        top.columnconfigure(1, weight=1)

        ttk.Label(top, text="External loader (flash externa):").grid(
            row=1, column=0, sticky="w", pady=(6, 0)
        )
        self.loader_combo = ttk.Combobox(
            top, textvariable=self.loader_var, state="readonly", width=77
        )
        self.loader_combo.grid(row=1, column=1, sticky="we", padx=4, pady=(6, 0))
        ttk.Button(top, text="Recargar", command=self._refresh_loaders).grid(
            row=1, column=2, pady=(6, 0)
        )

        ttk.Label(top, text="Connect mode:").grid(row=2, column=0, sticky="w", pady=(6, 0))
        adv = ttk.Frame(top)
        adv.grid(row=2, column=1, sticky="w", pady=(6, 0))
        ttk.Combobox(
            adv,
            textvariable=self.connect_mode_var,
            state="readonly",
            width=12,
            values=["UR", "HOTPLUG", "NORMAL"],
        ).pack(side="left")
        ttk.Label(adv, text="   Frecuencia SWD (KHz):").pack(side="left", padx=(12, 4))
        ttk.Entry(adv, textvariable=self.freq_var, width=8).pack(side="left")

        btns = ttk.Frame(self.root)
        btns.pack(fill="x", **pad)

        self.btn_generate = ttk.Button(
            btns,
            text="1) Generar FSBL_trusted.bin",
            command=self._on_generate_fsbl,
        )
        self.btn_generate.pack(side="left", padx=4)

        self.btn_generate_app = ttk.Button(
            btns,
            text="2) Generar AppNonSecure_trusted.bin",
            command=self._on_generate_appns,
        )
        self.btn_generate_app.pack(side="left", padx=4)

        self.btn_program = ttk.Button(
            btns,
            text=f"3) Programar FSBL firmado en {FSBL_FLASH_ADDRESS}",
            command=self._on_program_fsbl,
        )
        self.btn_program.pack(side="left", padx=4)

        self.btn_program_app = ttk.Button(
            btns,
            text=f"4) Programar AppNS firmado en {APPNS_FLASH_ADDRESS}",
            command=self._on_program_appns,
        )
        self.btn_program_app.pack(side="left", padx=4)

        self.btn_reset = ttk.Button(
            btns,
            text="Reset MCU",
            command=self._on_reset,
        )
        self.btn_reset.pack(side="left", padx=4)

        self.status_var = tk.StringVar(value="Listo.")
        ttk.Label(self.root, textvariable=self.status_var, foreground="#555").pack(
            fill="x", padx=10
        )

        log_frame = ttk.Frame(self.root)
        log_frame.pack(fill="both", expand=True, padx=8, pady=(0, 8))

        self.log_text = tk.Text(log_frame, wrap="word", state="disabled", bg="#111", fg="#ddd")
        self.log_text.pack(side="left", fill="both", expand=True)
        scroll = ttk.Scrollbar(log_frame, command=self.log_text.yview)
        scroll.pack(side="right", fill="y")
        self.log_text.configure(yscrollcommand=scroll.set)

    # ------------------------------------------------------------ helpers --

    def _log(self, line):
        self.log_queue.put(line)

    def _poll_log_queue(self):
        try:
            while True:
                line = self.log_queue.get_nowait()
                self.log_text.configure(state="normal")
                self.log_text.insert("end", line + "\n")
                self.log_text.see("end")
                self.log_text.configure(state="disabled")
        except queue.Empty:
            pass
        self.root.after(100, self._poll_log_queue)

    def _set_busy(self, busy, status=None):
        self.busy = busy
        state = "disabled" if busy else "normal"
        self.btn_generate.configure(state=state)
        self.btn_generate_app.configure(state=state)
        self.btn_program.configure(state=state)
        self.btn_program_app.configure(state=state)
        self.btn_reset.configure(state=state)
        if status:
            self.status_var.set(status)

    def _choose_project_dir(self):
        chosen = filedialog.askdirectory(initialdir=self.project_dir.get())
        if chosen:
            self.project_dir.set(chosen)
            self.cfg["project_dir"] = chosen
            save_config(self.cfg)
            self._refresh_loaders()

    def _project_dir(self, project_name):
        return os.path.join(self.project_dir.get(), project_name)

    def _build_dir(self, project_name):
        return os.path.join(self._project_dir(project_name), "build")

    def _toolchain_file(self):
        return os.path.join(self.project_dir.get(), "gcc-arm-none-eabi.cmake")

    # Loader preferido para este proyecto (NUCLEO-N657X0-Q): debe aparecer primero
    # en la lista y ser el default seleccionado.
    PREFERRED_LOADER = "MX25UM51245G_STM32N6570-NUCLEO.stldr"

    def _refresh_loaders(self):
        loaders = []
        if self.cubeprog_bin:
            loader_dir = os.path.join(self.cubeprog_bin, "ExternalLoader")
            loaders = sorted(
                os.path.basename(p) for p in glob.glob(os.path.join(loader_dir, "*.stldr"))
            )
            # Poner el loader preferido de primero en la lista, si existe
            if self.PREFERRED_LOADER in loaders:
                loaders.remove(self.PREFERRED_LOADER)
                loaders.insert(0, self.PREFERRED_LOADER)
        self.loader_combo["values"] = loaders
        if loaders:
            preferred = self.cfg.get("loader")
            if preferred and preferred in loaders:
                self.loader_var.set(preferred)
            elif self.PREFERRED_LOADER in loaders:
                self.loader_var.set(self.PREFERRED_LOADER)
            else:
                # Alternativa: cualquier loader de Nucleo si esta disponible
                default = next((l for l in loaders if "NUCLEO" in l.upper()), loaders[0])
                self.loader_var.set(default)
        else:
            self.loader_var.set("")
            self._log(
                "[AVISO] No se encontraron external loaders (.stldr). "
                "Verifica la instalacion de STM32CubeProgrammer."
            )

    def _find_trusted_bin(self, project_name="FSBL"):
        pattern = os.path.join(self._build_dir(project_name), "*_trusted.bin")
        matches = glob.glob(pattern)
        if not matches:
            return None
        return max(matches, key=os.path.getmtime)

    def _find_latest_file(self, directory, patterns):
        matches = []
        for pattern in patterns:
            matches.extend(glob.glob(os.path.join(directory, pattern)))
        if not matches:
            return None
        return max(matches, key=os.path.getmtime)

    def _objcopy_to_bin(self, elf_path):
        objcopy = find_objcopy()
        if not objcopy:
            raise FileNotFoundError(
                "No se encontro objcopy de la toolchain ARM en PATH. "
                "Instala arm-none-eabi-objcopy o añade la toolchain al PATH."
            )

        bin_path = os.path.splitext(elf_path)[0] + ".bin"
        cmd = [objcopy, "-O", "binary", "-S", elf_path, bin_path]
        self._log(f"\n$ {' '.join(cmd)}")
        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.stdout:
            self._log(result.stdout.rstrip())
        if result.stderr:
            self._log(result.stderr.rstrip())
        if result.returncode != 0:
            raise RuntimeError(f"Fallo al convertir ELF a BIN: {elf_path}")
        return bin_path

    def _sign_bin(self, raw_bin_path, output_bin_path, sign_type="fsbl"):
        if not self.cubeprog_bin or not os.path.isdir(self.cubeprog_bin):
            raise FileNotFoundError(
                "No se encontro la carpeta bin de STM32CubeProgrammer para usar el Signing Tool."
            )

        signing_tool = os.path.join(self.cubeprog_bin, SIGNING_TOOL_NAME)
        if not os.path.isfile(signing_tool):
            raise FileNotFoundError(
                f"No se encontro {SIGNING_TOOL_NAME} en: {self.cubeprog_bin}"
            )

        cmd = [
            signing_tool,
            "-bin",
            raw_bin_path,
            "-nk",
            "-of",
            EXTERNAL_FLASH_BASE,
            "-t",
            sign_type,
            "-hv",
            "2.3",
            "-align",
            "-o",
            output_bin_path,
            "-s",
        ]
        self._log(f"\n$ {' '.join(cmd)}")
        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.stdout:
            self._log(result.stdout.rstrip())
        if result.stderr:
            self._log(result.stderr.rstrip())
        if result.returncode != 0:
            raise RuntimeError(f"Fallo al firmar el binario: {raw_bin_path}")
        return output_bin_path

    def _build_project(self, project_name, on_done, friendly_name=None):
        project_name = project_name.strip()
        source_dir = self._project_dir(project_name)
        build_dir = self._build_dir(project_name)
        label = friendly_name or project_name

        if not os.path.isdir(source_dir):
            messagebox.showerror(
                "Carpeta invalida",
                f"No se encontro la carpeta {project_name} en:\n{source_dir}",
            )
            return

        def after_build(ok):
            on_done(ok, source_dir, build_dir)

        def after_configure(ok):
            if not ok:
                self._set_busy(False, f"Fallo la configuracion de {label}.")
                messagebox.showerror("Error", f"Fallo la configuracion de {label}. Revisa el log.")
                return
            self._run_command(["cmake", "--build", build_dir], cwd=source_dir, on_done=after_build)

        if os.path.isfile(os.path.join(build_dir, "CMakeCache.txt")):
            self._run_command(["cmake", "--build", build_dir], cwd=source_dir, on_done=after_build)
        else:
            self._log(f"Carpeta de build de {label} no configurada todavia, configurando...")
            cmd = [
                "cmake",
                "-S",
                source_dir,
                "-B",
                build_dir,
                "-G",
                "Ninja",
                f"-DCMAKE_TOOLCHAIN_FILE={self._toolchain_file()}",
                "-DCMAKE_BUILD_TYPE=Debug",
            ]
            self._run_command(cmd, cwd=source_dir, on_done=after_configure)

    def _prepare_signed_image(self, project_name, sign_type="fsbl"):
        build_dir = self._build_dir(project_name)
        trusted = self._find_trusted_bin(project_name)
        if trusted:
            return trusted

        raw_bin = self._find_latest_file(build_dir, ["*.bin"])
        if not raw_bin:
            latest_elf = self._find_latest_file(build_dir, ["*.elf"])
            if latest_elf:
                raw_bin = self._objcopy_to_bin(latest_elf)
        if not raw_bin:
            return None

        output_name = f"{os.path.splitext(os.path.basename(raw_bin))[0]}_trusted.bin"
        output_bin = os.path.join(os.path.dirname(raw_bin), output_name)
        return self._sign_bin(raw_bin, output_bin, sign_type=sign_type)

    def _program_image(self, image_path, flash_address, status_text, ok_text, fail_text):
        if not self.cubeprog_bin or not os.path.isdir(self.cubeprog_bin):
            messagebox.showerror(
                "STM32CubeProgrammer no encontrado",
                "No se encontro la carpeta bin de STM32CubeProgrammer.",
            )
            return

        loader_name = self.loader_var.get()
        if not loader_name:
            messagebox.showerror("Falta external loader", "Selecciona un external loader (.stldr).")
            return

        connect_mode = self.connect_mode_var.get() or "NORMAL"
        freq = self.freq_var.get().strip() or "8000"

        self.cfg["loader"] = loader_name
        self.cfg["cubeprog_bin"] = self.cubeprog_bin
        self.cfg["connect_mode"] = connect_mode
        self.cfg["freq"] = freq
        save_config(self.cfg)

        cli = os.path.join(self.cubeprog_bin, "STM32_Programmer_CLI.exe")
        loader_path = os.path.join(self.cubeprog_bin, "ExternalLoader", loader_name)

        cmd = [
            cli,
            "-c",
            "port=SWD",
            f"mode={connect_mode}",
            f"freq={freq}",
            "-el",
            loader_path,
            "-w",
            image_path,
            flash_address,
            "-v",
        ]

        self._set_busy(True, status_text)

        def after_flash(ok):
            if ok:
                self._set_busy(False, ok_text)
                messagebox.showinfo("Listo", ok_text)
                return

            self._set_busy(False, fail_text)
            self._log(
                "\n[SUGERENCIAS]\n"
                "  - Si el error es 'failed to erase memory': revisa los switches BOOT0/BOOT1.\n"
                "  - Si el error es 'Unable to run MCU' / 'MCU Reset Error': la escritura y\n"
                "    verificacion pueden haber funcionado; solo fallo el reset.\n"
                "  - Prueba bajar la frecuencia SWD a 1000-4000 KHz o cambiar el connect mode."
            )
            messagebox.showerror("Error de programacion", fail_text)

        self._run_command(cmd, cwd=self.cubeprog_bin, on_done=after_flash)

    # ------------------------------------------------------------ actions --

    def _run_command(self, cmd, cwd, on_done):
        """Ejecuta cmd en un hilo aparte, redirigiendo la salida al log."""

        def worker():
            self._log(f"\n$ {' '.join(cmd)}")
            try:
                proc = subprocess.Popen(
                    cmd,
                    cwd=cwd,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.STDOUT,
                    text=True,
                    bufsize=1,
                )
            except OSError as exc:
                self._log(f"[ERROR] No se pudo ejecutar el comando: {exc}")
                self.root.after(0, lambda: on_done(False))
                return

            for line in proc.stdout:
                self._log(line.rstrip())
            proc.wait()
            ok = proc.returncode == 0
            if not ok:
                self._log(f"[ERROR] Comando terminado con codigo {proc.returncode}")
            self.root.after(0, lambda: on_done(ok))

        threading.Thread(target=worker, daemon=True).start()

    def _on_generate_fsbl(self):
        if self.busy:
            return

        self._set_busy(True, "Compilando FSBL y generando trusted.bin...")

        def after_build(ok):
            if not ok:
                self._set_busy(False, "Fallo la compilacion.")
                messagebox.showerror("Error", "Fallo la compilacion. Revisa el log.")
                return
            trusted = self._find_trusted_bin("FSBL")
            if trusted:
                self._log(f"\nOK -> {trusted}")
                self._set_busy(False, f"trusted.bin generado: {os.path.basename(trusted)}")
            else:
                self._set_busy(
                    False,
                    "Compilacion OK pero no se encontro *_trusted.bin "
                    "(revisa STM32_SigningTool_CLI en el log).",
                )

        self._build_project("FSBL", after_build, friendly_name="FSBL")

    def _on_generate_appns(self):
        if self.busy:
            return

        def after_build(ok, source_dir, build_dir):
            if not ok:
                self._set_busy(False, "Fallo la compilacion de AppNonSecure.")
                messagebox.showerror(
                    "Error",
                    "Fallo la compilacion de AppNonSecure. Revisa el log y verifica que exista Secure_nsclib.",
                )
                return

            try:
                signed = self._prepare_signed_image("AppliNonSecure", sign_type="fsbl")
            except Exception as exc:
                self._set_busy(False, "Fallo la firma de AppNonSecure.")
                messagebox.showerror("Error", str(exc))
                return

            if signed:
                self._log(f"\nOK -> {signed}")
                self._set_busy(False, f"AppNS trusted.bin generado: {os.path.basename(signed)}")
            else:
                self._set_busy(False, "Compilacion OK pero no se encontro ningun .elf/.bin para AppNonSecure.")

        self._set_busy(True, "Compilando AppNonSecure y generando trusted.bin...")
        self._build_project("AppliNonSecure", after_build, friendly_name="AppNonSecure")

    def _on_program_fsbl(self):
        if self.busy:
            return

        trusted = self._find_trusted_bin("FSBL")
        if not trusted:
            messagebox.showerror(
                "Falta trusted.bin",
                "No se encontro ningun *_trusted.bin en FSBL/build.\n"
                "Presiona primero '1) Generar FSBL trusted.bin'.",
            )
            return

        self._program_image(
            image_path=trusted,
            flash_address=FSBL_FLASH_ADDRESS,
            status_text="Programando FSBL firmado via ST-LINK...",
            ok_text="FSBL programado y verificado correctamente.",
            fail_text="Fallo al programar el FSBL (revisa el log para sugerencias).",
        )

    def _on_program_appns(self):
        if self.busy:
            return

        self._set_busy(True, "Compilando AppNonSecure, firmando y preparando la programacion...")

        def after_build(ok, source_dir, build_dir):
            if not ok:
                self._set_busy(False, "Fallo la compilacion de AppNonSecure.")
                messagebox.showerror(
                    "Error",
                    "Fallo la compilacion de AppNonSecure. Revisa el log y verifica que exista Secure_nsclib.",
                )
                return

            try:
                appns = self._prepare_signed_image("AppliNonSecure", sign_type="fsbl")
            except Exception as exc:
                self._set_busy(False, "Fallo la firma de AppNonSecure.")
                messagebox.showerror("Error", str(exc))
                return

            if not appns:
                self._set_busy(False, "No se pudo generar el trusted.bin de AppNonSecure.")
                messagebox.showerror(
                    "Falta AppNS trusted.bin",
                    "No se encontro ni se pudo generar el trusted.bin de AppNonSecure.\n"
                    "Revisa la compilacion y el log.",
                )
                return

            self._program_image(
                image_path=appns,
                flash_address=APPNS_FLASH_ADDRESS,
                status_text="Programando AppNonSecure firmado via ST-LINK...",
                ok_text="AppNonSecure programado y verificado correctamente.",
                fail_text="Fallo al programar AppNonSecure (revisa el log para sugerencias).",
            )

        self._build_project("AppliNonSecure", after_build, friendly_name="AppNonSecure")

    def _on_reset(self):
        if self.busy:
            return
        if not self.cubeprog_bin or not os.path.isdir(self.cubeprog_bin):
            messagebox.showerror(
                "STM32CubeProgrammer no encontrado",
                "No se encontro la carpeta bin de STM32CubeProgrammer.",
            )
            return

        connect_mode = self.connect_mode_var.get() or "NORMAL"
        freq = self.freq_var.get().strip() or "8000"
        cli = os.path.join(self.cubeprog_bin, "STM32_Programmer_CLI.exe")

        cmd = [cli, "-c", "port=SWD", f"mode={connect_mode}", f"freq={freq}", "-rst"]

        self._set_busy(True, "Reiniciando el MCU...")

        def after_reset(ok):
            if ok:
                self._set_busy(False, "MCU reiniciado.")
            else:
                self._set_busy(False, "Fallo el reset (revisa el log).")

        self._run_command(cmd, cwd=self.cubeprog_bin, on_done=after_reset)


def main():
    root = tk.Tk()
    FlashToolApp(root)
    root.mainloop()


if __name__ == "__main__":
    if sys.platform != "win32":
        print("Este script fue pensado para Windows (rutas de STM32CubeProgrammer).")
    main()
