#!/usr/bin/env python
import os
import sys

# Importa o ambiente do godot-cpp
env = SConscript("godot-cpp/SConstruct")

# Adiciona múltiplos diretórios ao caminho de inclusão
env.Append(CPPPATH=[
    "src/",
])

# Coleta os arquivos-fonte usando Glob para melhor precisão
sources = []
sources.extend(Glob("src/*.cpp"))
sources.extend(Glob("src/core/*.cpp"))
sources.extend(Glob("src/characters/*.cpp"))
sources.extend(Glob("src/characters/player/*.cpp"))
sources.extend(Glob("src/ui/*.cpp"))
sources.extend(Glob("src/tools/*.cpp"))


if env["platform"] == "macos":
    library = env.SharedLibrary(
        "godot_editor/bin/libgdarpg.{}.{}.framework/libgdarpg.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            "godot_editor/bin/libgdarpg.{}.{}.simulator.a".format(
                env["platform"], env["target"]
            ),
            source=sources,
        )
    else:
        library = env.StaticLibrary(
            "godot_editor/bin/libgdarpg.{}.{}.a".format(
                env["platform"], env["target"]
            ),
            source=sources,
        )
else:
    library = env.SharedLibrary(
        "godot_editor/bin/libgdarpg{}{}".format(
            env["suffix"], env["SHLIBSUFFIX"]
        ),
        source=sources,
    )

Default(library)