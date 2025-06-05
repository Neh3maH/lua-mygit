local module = {}

function module.init(path)
	rm(path)
	mkdir(path)
end

function module.gss(path)
	print(" => git status: " .. path)
	return os.execute("cd " .. path .. " && git status -s")
end

return module
