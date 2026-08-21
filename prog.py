#!/usr/bin/env python3
"""
Flashea automaticamente el .elf del proyecto STM32 abierto.

Uso:
  python prog.py
  python prog.py --tool openocd
  python prog.py --elf build/Debug/LED_ARZ.elf
"""

from __future__ import annotations

import argparse
import os
import shutil
import subprocess
import sys
from pathlib import Path


def run(cmd: list[str]) -> None:
    print("$", " ".join(cmd))
    subprocess.run(cmd, check=True)


def find_elf(project_root: Path, elf_arg: str | None) -> Path:
    if elf_arg:
        elf_path = (project_root / elf_arg).resolve() if not Path(elf_arg).is_absolute() else Path(elf_arg)
        if not elf_path.exists():
            raise FileNotFoundError(f"No existe el archivo ELF indicado: {elf_path}")
        return elf_path

    search_patterns = [
        "build/Debug/*.elf",
        "build/**/Debug/*.elf",
        "build/**/*.elf",
        "**/*.elf",
    ]

    candidates: list[Path] = []
    for pattern in search_patterns:
        candidates.extend(project_root.glob(pattern))

    # Limpia duplicados y evita rutas internas de CMake.
    unique_candidates = []
    seen = set()
    for path in candidates:
        rp = path.resolve()
        if rp in seen:
            continue
        if "CMakeFiles" in rp.parts:
            continue
        seen.add(rp)
        unique_candidates.append(rp)

    if not unique_candidates:
        raise FileNotFoundError("No se encontro ningun .elf. Compila el proyecto primero.")

    # Prioriza el .elf mas reciente por fecha de modificacion.
    unique_candidates.sort(key=lambda p: p.stat().st_mtime, reverse=True)
    return unique_candidates[0]


def detect_tool(tool_arg: str | None) -> str:
    if tool_arg:
        return tool_arg

    env_tool = os.getenv("FLASH_TOOL")
    if env_tool:
        return env_tool

    if shutil.which("openocd"):
        return "openocd"
    if shutil.which("STM32_Programmer_CLI"):
        return "stm32cli"
    if shutil.which("st-flash"):
        return "st-flash"

    raise RuntimeError(
        "No se encontro herramienta de programacion. Instala openocd, "
        "STM32_Programmer_CLI o st-flash."
    )


def flash_with_openocd(elf_path: Path, interface_cfg: str, target_cfg: str) -> None:
    cmd = [
        "openocd",
        "-f",
        interface_cfg,
        "-f",
        target_cfg,
        "-c",
        f'program "{elf_path}" verify reset exit',
    ]
    run(cmd)


def flash_with_stm32cli(elf_path: Path) -> None:
    # STM32_Programmer_CLI suele aceptar ELF/HEX/BIN.
    cmd = [
        "STM32_Programmer_CLI",
        "-c",
        "port=SWD",
        "-w",
        str(elf_path),
        "-v",
        "-rst",
    ]
    run(cmd)


def flash_with_stflash(elf_path: Path) -> None:
    # st-flash trabaja mejor con BIN. Convierte ELF -> BIN si hay objcopy.
    objcopy = shutil.which("arm-none-eabi-objcopy") or shutil.which("llvm-objcopy")
    if objcopy:
        bin_path = elf_path.with_suffix(".bin")
        run([objcopy, "-O", "binary", str(elf_path), str(bin_path)])
        run(["st-flash", "--reset", "write", str(bin_path), "0x08000000"])
        return

    print(
        "Aviso: no se encontro objcopy. Se intentara flashear el ELF directo con st-flash."
    )
    run(["st-flash", "--reset", "write", str(elf_path), "0x08000000"])


def main() -> int:
    parser = argparse.ArgumentParser(description="Programa automaticamente el .elf del proyecto abierto.")
    parser.add_argument(
        "--project-root",
        default=str(Path.cwd()),
        help="Carpeta raiz del proyecto a escanear. Por defecto usa la carpeta actual.",
    )
    parser.add_argument("--elf", help="Ruta al .elf (opcional). Si no se indica, se detecta automaticamente.")
    parser.add_argument(
        "--tool",
        choices=["openocd", "stm32cli", "st-flash"],
        help="Herramienta de programacion a usar. Si no se indica, se detecta automaticamente.",
    )
    parser.add_argument(
        "--openocd-interface",
        default="interface/stlink.cfg",
        help="Archivo cfg de interfaz para openocd.",
    )
    parser.add_argument(
        "--openocd-target",
        default="target/stm32f4x.cfg",
        help="Archivo cfg de target para openocd.",
    )

    args = parser.parse_args()
    project_root = Path(args.project_root).resolve()

    try:
        elf_path = find_elf(project_root, args.elf)
        tool = detect_tool(args.tool)

        print(f"Proyecto: {project_root}")
        print(f"ELF seleccionado: {elf_path}")
        print(f"Programador: {tool}")

        if tool == "openocd":
            flash_with_openocd(elf_path, args.openocd_interface, args.openocd_target)
        elif tool == "stm32cli":
            flash_with_stm32cli(elf_path)
        elif tool == "st-flash":
            flash_with_stflash(elf_path)
        else:
            raise RuntimeError(f"Tool no soportada: {tool}")

        print("Programacion completada correctamente.")
        return 0

    except subprocess.CalledProcessError as exc:
        print(f"Error ejecutando comando: {exc}")
        return exc.returncode or 1
    except Exception as exc:
        print(f"Error: {exc}")
        return 1


if __name__ == "__main__":
    sys.exit(main())
