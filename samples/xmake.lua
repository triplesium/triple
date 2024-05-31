function add_sample(name)
    target("samples-" .. name)
        set_kind("binary")
        add_files(name .. ".cpp", "common/*.mpp")
        add_deps("triple")
        add_packages("imgui")
        -- set_targetdir("$(buildir)/samples")
end

add_sample("helloworld")
add_sample("assets")
add_sample("sprite")
add_sample("input")
add_sample("sprite_move")
add_sample("debug_draw")
add_sample("plane")
add_sample("physics")
add_sample("refl")

target("benchmark")
    set_kind("binary")
    add_files("benchmark.cpp")
    add_deps("triple")
    add_packages("catch2")
    set_targetdir("$(buildir)/samples")
