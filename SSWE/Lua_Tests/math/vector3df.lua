--LUA Tests for vector3df metatable

-----------------------------------------------------------------------------------------------
-------------------------------------------Create----------------------------------------------
-----------------------------------------------------------------------------------------------
--Create two vectors. Each vector is "0, 0, 0"
local vec1 = Vector3df.new()
local vec2 = Vector3df.new()

-----------------------------------------------------------------------------------------------
-------------------------------------------Getters----------------------------------------------
-----------------------------------------------------------------------------------------------

assert(vec1:x() == 0.0, "Creation : vec1:x() not equal to 0")
assert(vec1:y() == 0.0, "Creation : vec1:y() not equal to 0")
assert(vec1:z() == 0.0, "Creation : vec1:z() not equal to 0")

assert(vec2:x() == 0.0, "Creation : vec2:x() not equal to 0")
assert(vec2:y() == 0.0, "Creation : vec2:y() not equal to 0")
assert(vec2:z() == 0.0, "Creation : vec2:z() not equal to 0")

-----------------------------------------------------------------------------------------------
-------------------------------------------Setters---------------------------------------------
-----------------------------------------------------------------------------------------------
--Setters
vec1:setX(1.0)
vec1:setY(2.0)
vec1:setZ(3.0)
assert(vec1:x() == 1.0, "Setters setX : vec1:x() not equal to 1")
assert(vec1:y() == 2.0, "Setters setY : vec1:y() not equal to 2")
assert(vec1:z() == 3.0, "Setters setZ : vec1:z() not equal to 3")

vec2:setVector(vec1)
assert(vec2:x() == 1.0, "Setters setVector : vec2:x() not equal to 1")
assert(vec2:y() == 2.0, "Setters setVector : vec2:y() not equal to 2")
assert(vec2:z() == 3.0, "Setters setVector : vec2:z() not equal to 3")

vec1:setXYZ(0.0, 0.0, 0.0)
assert(vec1:x() == 0.0, "Setters setXYZ : vec1:x() not equal to 0")
assert(vec1:y() == 0.0, "Setters setXYZ : vec1:y() not equal to 0")
assert(vec1:z() == 0.0, "Setters setXYZ : vec1:z() not equal to 0")

-----------------------------------------------------------------------------------------------
----------------------------------------Operators----------------------------------------------
-----------------------------------------------------------------------------------------------
--Operators
vec1:setXYZ(1.0, 2.0, 3.0)
vec2:setXYZ(0.0, 0.0, 0.0)

--Add
vec2:add(vec1)
assert(vec2:x() == 1.0, "Operators add : vec2:x() not equal to 1")
assert(vec2:y() == 2.0, "Operators add : vec2:y() not equal to 2")
assert(vec2:z() == 3.0, "Operators add : vec2:z() not equal to 3")

vec2:addScalar(1.0)
assert(vec2:x() == 2.0, "Operators addScalar : vec2:x() not equal to 2")
assert(vec2:y() == 3.0, "Operators addScalar : vec2:y() not equal to 3")
assert(vec2:z() == 4.0, "Operators addScalar : vec2:z() not equal to 4")

--Multiply
vec2:setXYZ(0.0, 0.0, 0.0)
vec1:multiply(vec2)
assert(vec1:x() == 0.0, "Operators mul : vec1:x() not equal to 0")
assert(vec1:y() == 0.0, "Operators mul : vec1:y() not equal to 0")
assert(vec1:z() == 0.0, "Operators mul : vec1:z() not equal to 0")

vec1:setXYZ(1.0, 1.0, 1.0)
vec1:multiplyScalar(0.0)
assert(vec1:x() == 0.0, "Operators multiplyScalar : vec1:x() not equal to 0")
assert(vec1:y() == 0.0, "Operators multiplyScalar : vec1:y() not equal to 0")
assert(vec1:z() == 0.0, "Operators multiplyScalar : vec1:z() not equal to 0")

--Subtract
vec1:setXYZ(1.0, 1.0, 1.0)
vec2:setVector(vec1)
vec1:sub(vec2)
assert(vec1:x() == 0.0, "Operators sub : vec1:x() not equal to 0")
assert(vec1:y() == 0.0, "Operators sub : vec1:y() not equal to 0")
assert(vec1:z() == 0.0, "Operators sub : vec1:z() not equal to 0")

--Divide
vec1:setXYZ(1, 1, 1)
vec2:setXYZ(0, 0, 0)
vec2:setVector(Vector3df.new():addScalar(2.0)) --The magic is here =D
vec1:divide(vec2)
assert(vec1:x() == 0.5, "Operators divide : vec1:x() not equal to 0.5")
assert(vec1:y() == 0.5, "Operators divide : vec1:y() not equal to 0.5")
assert(vec1:z() == 0.5, "Operators divide : vec1:z() not equal to 0.5")

vec1:setXYZ(1, 1, 1)
vec1:divideScalar(2.0)
assert(vec1:x() == 0.5, "Operators divideScalar : vec1:x() not equal to 0.5")
assert(vec1:y() == 0.5, "Operators divideScalar : vec1:y() not equal to 0.5")
assert(vec1:z() == 0.5, "Operators divideScalar : vec1:z() not equal to 0.5")

-----------------------------------------------------------------------------------------------
------------------------------------------METHODS----------------------------------------------
-----------------------------------------------------------------------------------------------
--Dot Product
vec1:setXYZ(1, 1, 1)
vec2:setXYZ(2, 2, 2)
dot = vec1:dot(vec2)
assert(dot == 6, "Methods dot : dot not equal to 6")

-----------------------------------------------------------------------------------------------
-------------------------------------------Final-----------------------------------------------
-----------------------------------------------------------------------------------------------

--Finally
vec1:printVector()
vec2:printVector()





