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

FLASH_ADDRESS = "0x70000000"

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
        self.root.geometry("880x560")

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
            text="1) Generar trusted.bin",
            command=self._on_generate,
        )
        self.btn_generate.pack(side="left", padx=4)

        self.btn_program = ttk.Button(
            btns,
            text=f"2) Programar en flash externa ({FLASH_ADDRESS})",
            command=self._on_program,
        )
        self.btn_program.pack(side="left", padx=4)

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
        self.btn_program.configure(state=state)
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

    def _fsbl_dir(self):
        return os.path.join(self.project_dir.get(), "FSBL")

    def _build_dir(self):
        return os.path.join(self._fsbl_dir(), "build")

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

    def _find_trusted_bin(self):
        pattern = os.path.join(self._build_dir(), "*_trusted.bin")
        matches = glob.glob(pattern)
        if not matches:
            return None
        return max(matches, key=os.path.getmtime)

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

    def _on_generate(self):
        if self.busy:
            return
        fsbl_dir = self._fsbl_dir()
        build_dir = self._build_dir()

        if not os.path.isdir(fsbl_dir):
            messagebox.showerror(
                "Carpeta invalida",
                f"No se encontro la carpeta FSBL en:\n{fsbl_dir}",
            )
            return

        self._set_busy(True, "Compilando FSBL y generando trusted.bin...")

        def after_configure(ok):
            if not ok:
                self._set_busy(False, "Fallo la configuracion de CMake.")
                messagebox.showerror("Error", "Fallo la configuracion de CMake. Revisa el log.")
                return
            self._run_command(
                ["cmake", "--build", build_dir],
                cwd=fsbl_dir,
                on_done=after_build,
            )

        def after_build(ok):
            if not ok:
                self._set_busy(False, "Fallo la compilacion.")
                messagebox.showerror("Error", "Fallo la compilacion. Revisa el log.")
                return
            trusted = self._find_trusted_bin()
            if trusted:
                self._log(f"\nOK -> {trusted}")
                self._set_busy(False, f"trusted.bin generado: {os.path.basename(trusted)}")
            else:
                self._set_busy(
                    False,
                    "Compilacion OK pero no se encontro *_trusted.bin "
                    "(revisa STM32_SigningTool_CLI en el log).",
                )

        if os.path.isfile(os.path.join(build_dir, "CMakeCache.txt")):
            self._run_command(["cmake", "--build", build_dir], cwd=fsbl_dir, on_done=after_build)
        else:
            self._log("Carpeta de build no configurada todavia, configurando...")
            cmd = [
                "cmake",
                "-S",
                fsbl_dir,
                "-B",
                build_dir,
                "-G",
                "Ninja",
                f"-DCMAKE_TOOLCHAIN_FILE={self._toolchain_file()}",
                "-DCMAKE_BUILD_TYPE=Debug",
            ]
            self._run_command(cmd, cwd=fsbl_dir, on_done=after_configure)

    def _on_program(self):
        if self.busy:
            return

        trusted = self._find_trusted_bin()
        if not trusted:
            messagebox.showerror(
                "Falta trusted.bin",
                "No se encontro ningun *_trusted.bin en FSBL/build.\n"
                "Presiona primero '1) Generar trusted.bin'.",
            )
            return

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
            trusted,
            FLASH_ADDRESS,
            "-v",
        ]

        self._set_busy(True, "Programando flash externa via ST-LINK...")

        def after_flash(ok):
            if ok:
                self._set_busy(False, "Programacion completada correctamente.")
                messagebox.showinfo(
                    "Listo",
                    "Binario programado y verificado en la flash externa.\n"
                    "Si quieres que el MCU corra ahora, usa el boton 'Reset MCU', "
                    "o cambia los switches BOOT a modo flash y presiona el boton de reset fisico.",
                )
            else:
                self._set_busy(False, "Fallo la programacion. Revisa el log.")
                self._log(
                    "\n[SUGERENCIAS]\n"
                    "  - Si el error es 'failed to erase memory': switches BOOT0/BOOT1 deben\n"
                    "    estar en modo desarrollo (BOOT1=2-3) mientras programas por ST-LINK.\n"
                    "  - Si el error es 'Unable to run MCU' / 'MCU Reset Error': la escritura y\n"
                    "    verificacion probablemente SI funcionaron; solo fallo el intento de\n"
                    "    reiniciar y correr el MCU justo despues. Usa el boton 'Reset MCU' por\n"
                    "    separado, o ignora el error si solo vas a probar el boot fisicamente.\n"
                    "  - Prueba bajar la frecuencia SWD a 1000-4000 KHz o cambiar el connect "
                    "mode (NORMAL/UR/HOTPLUG)."
                )
                messagebox.showerror(
                    "Error de programacion",
                    "Fallo al programar o al reiniciar el MCU (revisa el log para "
                    "sugerencias).",
                )

        self._run_command(cmd, cwd=self.cubeprog_bin, on_done=after_flash)

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
