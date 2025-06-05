local types = require 'types'

local module = {}

function module.rm(path)
	assert(types.is.str(path) and path ~= "", "rm invalid path")
	return os.execute("rm -rf " .. path)
end

function module.mkdir(path)
	assert(types.is.str(path) and path ~= "", "mkdir invalid path")
	return os.execute("mkdir -p " .. path)
end

return module
