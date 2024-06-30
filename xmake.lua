---@diagnostic disable

add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", { outputdir = ".vscode" })

add_requires("pybind11")
add_requires("rime")

target("pyrime", function()
	add_rules("python.library")
	add_files("src/*.cc")

	add_packages("pybind11")
	add_packages("rime")
	set_languages("c++17")
	set_installdir(".")
end)
