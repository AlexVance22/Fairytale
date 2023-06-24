require("engine")


hitpoints = 100


function OnUpdate(other)
    if hitpoints <= 0 then
        Self:Destroy()
    end
end