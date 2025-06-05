local types = require 'lua.utils.types'

local module = {}

function module.factory(v)
	return function() return helpers.cpy(v) end 
end

function module.id(v)
	return v
end

function module.cpy(v)
	if types.is.tbl(v) then
		local new = {}
		for k, v in pairs(v) do
			new[module.cpy(k)] = module.cpy(v)
		end
		return new
	else
		return v
	end
end

return module
