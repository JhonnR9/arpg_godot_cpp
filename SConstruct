#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

env.Append(CPPPATH=[
    "arpg/",
])


sources = []

sources.extend(Glob("arpg/*.cpp"))
sources.extend(Glob("arpg/tools/*.cpp"))

if env["target"] in ["editor", "template_debug"]:
    try:
        doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
        sources.append(doc_data)
    except AttributeError:
        print("Not including class reference as we're targeting a pre-4.3 baseline.")

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