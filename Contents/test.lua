local test =
{
}

function test:OnStart()
    
end

function test:OnUpdate(deltaTime)
    if Input.GetKey(Key.A) then	
        a = self.owner:GetTransform():GetPosition()
        offset = Vector3.new(deltaTime, 0, 0)
        self.owner:GetTransform():SetPosition(a + offset)
    end
end

return test