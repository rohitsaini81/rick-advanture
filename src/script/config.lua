title = "My Rick and Morty"
width = 800
height = 600
health = 1400
playerMovementSpeed = 10
fullscreen = false

return {
    version = "1.0.0",

    say_hello = function(name)
        print("Hello you from Lua, " .. name .. "!")
    end,

    print_health = function()
        print("Current health is: " .. tostring(health))
    end
}
