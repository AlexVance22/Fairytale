require "engine"


local RigidBody = nil
local Animation = nil
local TreesInRange = {}

Locked = false


function OnCreate()
    RigidBody = Self:GetComponentRef(ECS.Comp.RIGIDBODY)
    Animation = Self:GetComponentRef(ECS.Comp.ANIMATION)
end


function OnUpdate(deltatime)
    if not Locked then
        local inputs = {
            x = (Input.QueryKey(Input.Keyboard.D) - Input.QueryKey(Input.Keyboard.A)) * 3,
            y = (Input.QueryKey(Input.Keyboard.S) - Input.QueryKey(Input.Keyboard.W)) * 3
        }

        if inputs.x ~= 0 and inputs.y ~= 0 then
            inputs.x = inputs.x * Physics.DiagScalar;
            inputs.y = inputs.y * Physics.DiagScalar;
        end

        RigidBody:SetVelocity(inputs)
    end
end


function OnHit(other, thiscol, othercol)
    local layer = other:GetComponentVal(ECS.Comp.LAYER)
    local col_id = thiscol:GetID()

    if layer == "tree" and col_id == "axe-range" then
        local tag = other:GetComponentVal(ECS.Comp.TAG)
        if TreesInRange[tag] == nil then
            TreesInRange[tag] = {
                entity = other, coll = othercol,
                anim = other:GetComponentRef(ECS.Comp.ANIMATION),
                script = other:GetComponentRef(ECS.Comp.SCRIPT) 
            }
        end
        TreesInRange[tag].anim:SetAnimation(1)
    end
end


function OnExit(other, thiscol, othercol)
    local layer = other:GetComponentVal(ECS.Comp.LAYER)
    local col_id = thiscol:GetID()

    if layer == "tree" and col_id == "axe-range" then
        local tag = other:GetComponentVal(ECS.Comp.TAG)
        if TreesInRange[tag] ~= nil then
            TreesInRange[tag].anim:SetAnimation(0)
            TreesInRange[tag] = nil
        end
    end
end


function OnMouseClickAsync(pos, mouse)
    local physpos = ScreenToPhys(Scene, pos)

    for key, val in pairs(TreesInRange) do

        if val.coll:TestPoint(physpos) then

            local hp = val.script:GetVar("hitpoints", Engine.Types.INT) - 50
            val.script:SetVar("hitpoints", Engine.Types.INT, hp)
        
            if hp == 0 then
                TreesInRange[key] = nil
            end
        end
    end

    Animation:SetAnimation(1)
    coroutine.yield(1)
    Animation:SetAnimation(0)
end