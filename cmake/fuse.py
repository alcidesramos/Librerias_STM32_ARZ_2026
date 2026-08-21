#!/usr/bin/env python3
"""Interfaz visual para configurar RDP (AA/BB/CC) en STM32."""

from __future__ import annotations

import shutil
import subprocess
import sys
import threading
import tkinter as tk
from tkinter import messagebox
from tkinter import scrolledtext

RDP_MAP = {
    "AA": "0xAA",  # Level 0
    "BB": "0xBB",  # Level 1
    "CC": "0xCC",  # Level 2 (irreversible)
}


def run_capture(cmd: list[str]) -> str:
    completed = subprocess.run(cmd, check=True, capture_output=True, text=True)
    out = completed.stdout.strip()
    err = completed.stderr.strip()
    if out and err:
        return f"{out}\n{err}"
    if out:
        return out
    if err:
        return err
    return "(sin salida)"


def check_cli() -> str:
    cli = shutil.which("STM32_Programmer_CLI")
    if not cli:
        raise RuntimeError(
            "No se encontro STM32_Programmer_CLI en PATH. "
            "Instala STM32CubeProgrammer y agrega su carpeta al PATH."
        )
    return cli


def show_current_rdp(cli: str) -> str:
    return run_capture([cli, "-c", "port=SWD", "-ob", "displ"])


def set_rdp(cli: str, rdp_hex: str) -> str:
    return run_capture([cli, "-c", "port=SWD", "-ob", f"RDP={rdp_hex}"])


class FuseApp:
    def __init__(self, root: tk.Tk) -> None:
        self.root = root
        self.root.title("STM32 Fuse Tool ARZ")
        self.root.geometry("760x520")
        self.root.minsize(700, 480)

        self.selected_rdp = tk.StringVar(value="AA")
        self.selected_rdp.trace_add("write", self.on_selection_changed)

        title = tk.Label(
            root,
            text="Proteccion RDP STM32 ARZ",
            font=("Segoe UI", 16, "bold"),
            pady=10,
        )
        title.pack()

        frame = tk.LabelFrame(root, text="Seleccion de Fuse", padx=14, pady=12)
        frame.pack(fill="x", padx=12, pady=8)

        tk.Radiobutton(
            frame,
            text="AA - Level 0 (sin proteccion)",
            variable=self.selected_rdp,
            value="AA",
            font=("Segoe UI", 11),
        ).pack(anchor="w")

        tk.Radiobutton(
            frame,
            text="BB - Level 1 (protegido)",
            variable=self.selected_rdp,
            value="BB",
            font=("Segoe UI", 11),
        ).pack(anchor="w")

        tk.Radiobutton(
            frame,
            text="CC - Level 2 (permanente)",
            variable=self.selected_rdp,
            value="CC",
            font=("Segoe UI", 11),
        ).pack(anchor="w")

        button_row = tk.Frame(root)
        button_row.pack(fill="x", padx=12, pady=8)

        self.send_button = tk.Button(
            button_row,
            text="Enviar Fuse",
            font=("Segoe UI", 11, "bold"),
            bg="#0078D4",
            fg="white",
            activebackground="#106EBE",
            activeforeground="white",
            padx=18,
            pady=8,
            command=self.on_send_clicked,
        )
        self.send_button.pack(side="left")
        self.update_send_button_style()

        self.log = scrolledtext.ScrolledText(
            root,
            wrap="word",
            height=18,
            font=("Consolas", 10),
        )
        self.log.pack(fill="both", expand=True, padx=12, pady=(4, 12))
        self.log.insert("end", "Listo. Selecciona AA/BB/CC y presiona 'Enviar Fuse'.\n")
        self.log.configure(state="disabled")

    def append_log(self, text: str) -> None:
        self.log.configure(state="normal")
        self.log.insert("end", text + "\n")
        self.log.see("end")
        self.log.configure(state="disabled")

    def set_busy(self, busy: bool) -> None:
        self.send_button.configure(state="disabled" if busy else "normal")

    def on_selection_changed(self, *_args: object) -> None:
        self.update_send_button_style()

    def update_send_button_style(self) -> None:
        if self.selected_rdp.get() == "CC":
            self.send_button.configure(bg="#C62828", activebackground="#B71C1C")
        else:
            self.send_button.configure(bg="#0078D4", activebackground="#106EBE")

    def on_send_clicked(self) -> None:
        selected = self.selected_rdp.get()
        if selected == "CC":
            ok = messagebox.askokcancel(
                "Confirmacion Critica",
                "ADVERTENCIA: CC (RDP Level 2) es PERMANENTE en muchos STM32.\n"
                "Puede bloquear lectura y depuracion de forma irreversible.\n\n"
                "Presiona Aceptar para continuar o Cancelar para salir.",
                icon="warning",
            )
            if not ok:
                self.append_log("Operacion cancelada por usuario.")
                return

        self.set_busy(True)
        self.append_log(f"Aplicando seleccion {selected}...")

        worker = threading.Thread(target=self.apply_fuse, args=(selected,), daemon=True)
        worker.start()

    def apply_fuse(self, selected: str) -> None:
        try:
            cli = check_cli()
            rdp_hex = RDP_MAP[selected]

            before = show_current_rdp(cli)
            apply_result = set_rdp(cli, rdp_hex)
            after = show_current_rdp(cli)

            result_text = [
                f"Seleccionado: {selected} ({rdp_hex})",
                "",
                "Estado actual:",
                before,
                "",
                "Aplicacion:",
                apply_result,
                "",
                "Estado final:",
                after,
                "",
                "Operacion completada.",
            ]

            self.root.after(0, lambda: self.append_log("\n".join(result_text)))
            self.root.after(0, lambda: messagebox.showinfo("Fuse", "Fuse aplicado correctamente."))

        except subprocess.CalledProcessError as exc:
            self.root.after(0, lambda: self.append_log(f"Error en comando: {exc}"))
            self.root.after(
                0,
                lambda: messagebox.showerror(
                    "Error",
                    "Fallo al ejecutar STM32_Programmer_CLI.\n"
                    "Revisa el panel de log para mas detalles.",
                ),
            )
        except Exception as exc:
            self.root.after(0, lambda: self.append_log(f"Error: {exc}"))
            self.root.after(0, lambda: messagebox.showerror("Error", str(exc)))
        finally:
            self.root.after(0, lambda: self.set_busy(False))


def main() -> int:
    root = tk.Tk()
    FuseApp(root)
    root.mainloop()
    return 0


if __name__ == "__main__":
    sys.exit(main())
