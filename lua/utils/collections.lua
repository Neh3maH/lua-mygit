local utils = require 'lua.utils.misc'
local module = {}

local function add_fun(name, fun)
	module[name] = function(tbl, ...) return fun(pairs, tbl, select('1', ...)) end
	module['i' .. name] = function(tbl, ...) return fun(ipairs, tbl, select('1', ...)) end
end

local function map(iter, tbl, fun)
	local ret = {}
	for k, v in iter(tbl) do
		ret[k] = fun(v, k)
	end

	return ret
end
add_fun('map', map)

local function disp(iter, tbl)
	map(iter, tbl, print)
end
add_fun('disp', disp)

local function find(iter, tbl, fun)
	for k, v in iter(tbl) do
		if fun(v, k) then
			return utils.cpy(k)
		end
	end
end
add_fun('find', find)

local function findk(iter, tbl, val)
	local f = function(v) return v == val end
	return find(iter, tbl, f)
end
add_fun('findk', findk)

local function fold(iter, tbl, fun, acc)
	for k, v in iter(tbl) do
		acc = fun(acc, v, k)
	end

	return acc
end
add_fun('fold', fold)

function module.rev(tbl)
	local new = {}
	local i = #tbl
	while i ~= 0 do
		table.insert(new, utils.cpy(tbl[i]))
		i = i - 1
	end

	return new
end

function module.pop(tbl)
	local len = #tbl
	local ret = tbl[len]
	tbl[len] = nil
	return ret
end

return module
