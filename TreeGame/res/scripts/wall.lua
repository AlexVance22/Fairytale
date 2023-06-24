require "engine"


local Player = nil
local PlayerRB = nil
local PlayerSC = nil


function OnCreate()
    Player = FindEntityByTag(Scene, "player")
    PlayerRB = Player:GetComponentRef(ECS.Comp.RIGIDBODY)
    PlayerSC = Player:GetComponentRef(ECS.Comp.SCRIPT)
end


function isPlayer(entity)
    if Player.id == entity.id and Player.scene == entity.scene then
        return true
    else
        return false
    end
end


function OnHitAsync(other, thiscol, othercol)
    if isPlayer(other) then
        PlayerSC:SetVar("Locked", Engine.Types.BOOL, true)
        PlayerRB:SetVelocity({ x = -3, y = 0 })
        coroutine.yield(1)
        PlayerSC:SetVar("Locked", Engine.Types.BOOL, false)
    end
end