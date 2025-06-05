assert(package.loadlib('./libs/libgit2.so.1.9', '*'))
require 'toast'
local helpers = require 'lua.helpers'
local types = require 'lua.utils.types'
local utils = require 'lua.utils.misc'
local collections = require 'lua.utils.collections'
local argparse = require 'lua.argparse'

print("==== ARGS ====")
--if nil == res then
	--print("==== RES: ERROR ====")
--else
	--print("==== RES: SUCCESS ====")
	--print(" - opts:", #res.options)
	--print(" - params:", #res.params)
	--print("")
	--print("==== OPTS ====")
	--collections.disp(res.options)
	--print("")
	--print("==== PARAMS ====")
	--collections.disp(res.params)
	--print("==== ====")
--end

print('\nlet\'s gong!')

local steps = {}

function steps.print_lib(str)
	collections.disp(toastlib)
	toastlib.hello_world(str)
end

function steps.delete_repo(path)
	init(path)
	return err
end

function steps.clone(url, path)
	err = toastlib.clone(url, path)
	print("clone = " .. tostring(err))
	return err
end

function steps.list_branches(path)
	err = toastlib.list_branches(path);
	print("list_branches = " .. tostring(err))
	return err
end

function steps.fetch(path)
	err = toastlib.fetch(path)
	print("fetch = " .. tostring(err))
	return err
end

function steps.checkout(refish, path)
	err, loc = toastlib.checkout(refish, path)
	print("checkout " .. tostring(refish) .. " = " .. tostring(err) .. " .. " .. tostring(loc))
	return err
end

steps.branch = {}
function steps.branch.create_branch(bname, path)
	err = toastlib.create_branch(bname, path)
	print("branch_create " .. bname .. " = " .. tostring(err))
	return err
end

function steps.branch.move(src, dst, path)
	--TODO: switch params
	err = toastlib.move_branch(dst, src, path)
	print("move " .. tostring(src) .. " -> " .. tostring(dst) .. " = " .. tostring(err))
	return err
end

function steps.branch.delete(bname, path)
	err = toastlib.delete_branch(bname, path);
	print("delete " .. tostring(bname) .. " = " .. tostring(err))
	return err
end

steps.index = {}
function steps.index.add(fname, path)
	err = toastlib.add(testfile, path)
	print("add " .. tostring(fname) .. " = " .. tostring(err))
	return err
end

function steps.index.add_all(path)
	err = toastlib.add(nil, path)
	print("add all = " .. tostring(err))
	return err
end

function steps.index.remove(fname, path)
	err = toastlib.remove(fname, path)
	print("remove " .. tostring(fname) .. " = " .. tostring(err))
	return err
end

steps.commit = {}
function steps.commit.create(msg, path)
	err = toastlib.commit(msg, path)
	print("commit = (" .. tostring(err) .. ") " .. tostring(msg))
	return err
end

function steps.commit.amend(msg, path)
	err = toastlib.amend(msg, path)
	print("amend = (" .. tostring(err) .. ") " .. msg)
	return err
end

local _url = "git@github.com:Neh3maH/llama_minecraft.git"
local _path = "./tmp"

toastlib.init()
--os.execute('rm -rf ./tmp')
--steps.clone(_url, _path)
--os.execute("echo vermicelle > ./tmp/newfile")
--os.execute("echo edited > ./tmp/toast")
--steps.index.add_all(_path)
--steps.commit.create("new commit", _path)
os.execute("echo vermicelle1 > ./tmp/newfile1")
os.execute("echo edited > ./tmp/newfile")
os.execute("rm -rf ./tmp/toast")
err = toastlib.reset_hard("main", _path)
print("err = ", err)
toastlib.shutdown()
