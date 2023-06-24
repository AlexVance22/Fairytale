require "engine"


local Player = nil
local RigidBody = nil


function OnCreate()
    Player = FindEntityByTag(Scene, "player")
    RigidBody = Self:GetComponentRef(ECS.Comp.RIGIDBODY)
    Log.msg(Player:GetComponentVal(ECS.Comp.TAG))
end


function isPlayer(entity)
    if Player.id == entity.id and Player.scene == entity.scene then
        return true
    else
        return false
    end
end


function OnUpdateAsync(deltatime)
    Log.msg("Enemy go right")
    RigidBody:SetVelocity({ x = 2, y = 0 })
    coroutine.yield(2)
    Log.msg("Enemy go left")
    RigidBody:SetVelocity({ x = -2, y = 0 })
    return 2
end


function OnHitAsync(other, thiscol, othercol)
    if isPlayer(other) then
        local anim = Player:GetComponentRef(ECS.Comp.ANIMATION)
        Log.msg("Set player anim hit")
        anim:SetAnimation(2)
        coroutine.yield(1)
        Log.msg("Set player anim default")
        anim:SetAnimation(0)
        return 0
    end
end