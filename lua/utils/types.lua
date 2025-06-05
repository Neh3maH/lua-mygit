local module = {}
module.is = {}
module.ass = {}

local function is_t(v, t)
	return t == type(v)
end

local function ass_t(v, t)
	local err_str = string.format('%s is not a %s', tostring(v), module[k])
	return assert(module.is[k](v), err_str)
end

local function add_type(k, t)
	module[k] = t
	module[t] = k
	module.is[k] = function(v) return is_t(v, module[k]) end
	module.is[t] = module.is[k]
	module.ass[k] = function(v) return ass_t(v, module[k]) end
	module.ass[t] = module.ass[k]
end

add_type('n', 'nil')
add_type('str', 'string')
add_type('tbl', 'table')
add_type('num', 'number')
add_type('bool', 'boolean')
add_type('fun', 'function')
add_type('udat', 'userdata')
add_type('thread', 'thread')

return module
