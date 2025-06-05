local module = {}

__memoize_inputs = {}

function module.req(name)
	assert(type(name) == 'string' and #name > 0)
	local old = __memoize_inputs[name]

	if old then
		return old
	end

	local new = require(name)
	__memoize_inputs[name] = new
	return new
end

return module
