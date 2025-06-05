local module = {}

module.CmdParser = require 'lua.argparse.cmd_parser'
module.Option = require 'lua.argparse.option'
module.Option.keys = require 'lua.argparse.opt_k'
module.Option.values = require 'lua.argparse.opt_v'
module.validators = require 'lua.argparse.validators'
module.Parser = require 'lua.argparse.parser'

function module.new_parser(subcmds)
	return module.Parser:new(subcmds)
end

return module
