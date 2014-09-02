--LUA Tests for vector3df metatable

-----------------------------------------------------------------------------------------------
-------------------------------------------Create----------------------------------------------
-----------------------------------------------------------------------------------------------
--Create two vectors. Each vector is "0, 0, 0"
local mat1 = Matrix4.new()
local mat2 = Matrix4.new()

mat2:add(mat1)
-----------------------------------------------------------------------------------------------
-------------------------------------------Getters----------------------------------------------
-----------------------------------------------------------------------------------------------
pointer1 = mat1:pointer()
assert(pointer1[0] == 1.0, "Creation : mat1:pointer()[0] not equal to 1.0, equals "..pointer1[0])

-----------------------------------------------------------------------------------------------
----------------------------------------Operators----------------------------------------------
-----------------------------------------------------------------------------------------------
--Set
local temp_mat = Matrix4.new()
local temp_mat2 = Matrix4.new()
temp_mat:set(temp_mat2)
assert(temp_mat:equals(temp_mat2))

--Equals
mat2 = Matrix4.new()
local mat3 = Matrix4.new()

assert(mat2:equals(mat3), "mat1 equals mat2")

--Addition
mat1:add(mat2)
assert(not mat1:equals(mat2), "Matrix4:add mat1 and mat2 equality is false")
mat1:minus(mat2)
--Minux
assert(mat1:equals(mat2), "Matrix4:minus: mat1 and mat2 equality is true")
--Multiply
mat1:multiply(mat2)
assert(mat1:equals(mat2), "Matrix4:multiply: mat1 and mat2 equality is true")

-----------------------------------------------------------------------------------------------
------------------------------------------METHODS----------------------------------------------
-----------------------------------------------------------------------------------------------
--Identity & Orthogonal
assert(mat1:makeIdentity():equals(mat2), "Matrix4:makeIdentity: mat1 and mat2 equality is true")
assert(mat1:isIdentity(), "Matrix4:isIdentity: is true")

assert(mat1:isOrthogonal() == true, "Matrix4:isOrthogonal: is true")
mat1:add(mat2)
assert(mat1:isOrthogonal() and not mat1:equals(mat2) == true, "Matrix4:isOrthogonal: is true")

assert(mat1:isIdentity_integer_base() == false, "Matrix4:isIdentity_integer_base: is false")

--Translation
local v = Vector3df.new()
mat1:makeIdentity()
mat3:makeIdentity()
mat1:setTranslation(v)
assert(mat1:equals(mat3), "mat1:setTranslation: must be equals to mat 3")
mat1:setTranslation(v:setX(10.0))
assert(mat1:equals(mat3) == false)

v = Vector3df.new()
mat1:getTranslation(v)
assert(v:x() == 10.0, "mat1:getTranslation: v:x() must be equals to 10.0")

mat1:setRotationRadians(v)
mat1:setRotationDegrees(v)
mat1:getRotationDegrees(v)
mat1:setInverseRotationRadians(v)
mat1:setInverseRotationDegrees(v)
mat1:setRotationAxisRadians(10.0, v)

mat1:setScale(v)
mat1:setScale(10.0)
mat1:getScale(v)

local matWorld = Matrix4.new():setWorld()
local matView = Matrix4.new():setView()
local matProj = Matrix4.new():setProj()

assert(matWorld:multiply(matView):multiply(matProj):equals(mat1:setWorldViewProj()), "world view proh concatenation doesn't work properly")

-----------------------------------------------------------------------------------------------
-------------------------------------------Setters---------------------------------------------
-----------------------------------------------------------------------------------------------
--Setters


-----------------------------------------------------------------------------------------------
-------------------------------------------Final-----------------------------------------------
-----------------------------------------------------------------------------------------------
local finalMat = Matrix4.new()
local finalVec = Vector3df.new()
finalMat:multiply(finalMat:setWorld()):multiply(finalMat:setView()):multiply(finalMat:setProj()):setWorldViewProj():setRotationDegrees(finalVec):add(finalMat)
finalMat:printMatrix()
