-- "constants"
local VERSION_INFO_FILE = ".vinfo"
local RESOURCE_FORMAT = [[
#define IDR_VERSION1 1

IDR_VERSION1 VERSIONINFO
FILEVERSION %d,%d,%d,%d
PRODUCTVERSION %d,%d,%d,%d
FILEOS 0x00000004
FILETYPE 0x00000001
BEGIN
  BLOCK "StringFileInfo"
  BEGIN
    BLOCK "FFFF0000"
    BEGIN
      VALUE "FileVersion", "%s\0"
      VALUE "ProductVersion", "%s\0"
      VALUE "CompanyName", "%s\0"
      VALUE "FileDescription", "%s\0"
      VALUE "InternalName", "%s\0"
      VALUE "OriginalFilename", "%s\0"
      VALUE "ProductName", "%s\0"
    END
  END
  BLOCK "VarFileInfo"
  BEGIN
    VALUE "Translation", 0xFFFF, 0x0000
  END
END

]]
local HEADER_FORMAT = [[
#ifndef VERSION_HEADER_%s
#define VERSION_HEADER_%s

#define VERSION_MAJOR %d
#define VERSION_MINOR %d
#define VERSION_PATCH %d
#define VERSION_BUILD %d

#define VERSION_STRING "%s"

#define VERSION_RC_STR "%s"
#define VERSION_RC_PLAIN %s

#endif
]]
local VERSION_FORMAT = "%d.%d.%d.%d"
local VERSION_RC_FMT = "%d,%d,%d,%d"
local VERSION_NICE_FORMAT = "%d-%d-%d-%d"
local VERSION_MATCH = "(%d+)%.(%d+)%.(%d+)%.(%d+)"
local VERSION_NICE_MATCH = "(%d+)%-(%d+)%-(%d+)%-(%d+)"

local LOWER_FIELDS = {
	major = "minor",
	minor = "patch",
}

------

local VersionInfo = {}

local function LoadVInfo()
	local file = io.open(VERSION_INFO_FILE, "r")
	
	local name = file:read("*l")
	VersionInfo.name = name
	
	local app = file:read("*l")
	VersionInfo.file = app
	
	local author = file:read("*l")
	VersionInfo.author = author
	
	local version = file:read("*l")
	local major, minor, patch, build = version:match(VERSION_MATCH)
	
	VersionInfo.major = major
	VersionInfo.minor = minor
	VersionInfo.patch = patch
	VersionInfo.build = build
end

local function SaveVInfo()
	local file = io.open(VERSION_INFO_FILE, "w")
	
	file:write(VersionInfo.name, "\n")
	file:write(VersionInfo.file, "\n")
	file:write(VersionInfo.author, "\n")
	file:write(VERSION_FORMAT:format(VersionInfo.major, VersionInfo.minor, VersionInfo.patch, VersionInfo.build), "\n")
end

local Commands = {
	inc = function(field)
		LoadVInfo()
		
		field = field:lower()
		
		if not VersionInfo[field] then
			print("Unknown field: " .. field)
		else
			VersionInfo[field] = VersionInfo[field] + 1			
			
			-- Reset minor/patch field on higher change
			local lower = LOWER_FIELDS[field]
			while (lower) do
				VersionInfo[lower] = 0
				lower = LOWER_FIELDS[lower]
			end
			
			SaveVInfo()
		end
	end,
	
	set = function(field, value)
		LoadVInfo()
		
		VersionInfo[field] = value
		
		SaveVInfo()
	end,
	
	write_res = function(fname)
		LoadVInfo()
		
		local file = io.open(fname, "w")
		
		local name, app, author = VersionInfo.name, VersionInfo.file, VersionInfo.author
		local ma,mi,p,b = VersionInfo.major, VersionInfo.minor, VersionInfo.patch, VersionInfo.build
		
		file:write(RESOURCE_FORMAT:format(
			ma, mi, p, b,
			ma, mi, p, b,
			VERSION_FORMAT:format(ma,mi,p,b),
			VERSION_FORMAT:format(ma,mi,p,b),
			author,
			app,
			app,
			app,
			name
		))
	end,
	
	write_header = function(fname)
		LoadVInfo()
		
		local file = io.open(fname, "w")
		
		local name, app, author = VersionInfo.name, VersionInfo.file, VersionInfo.author
		local ma,mi,p,b = VersionInfo.major, VersionInfo.minor, VersionInfo.patch, VersionInfo.build

		local niceName = fname:gsub("%.", "_")

		file:write(HEADER_FORMAT:format(
			niceName, niceName,
			ma, mi, p, b,
			VERSION_FORMAT:format(ma,mi,p,b),
			VERSION_RC_FMT:format(ma,mi,p,b),
			VERSION_RC_FMT:format(ma,mi,p,b)
		))
	end,
	
	write_version = function(fin, fout)
		LoadVInfo()
		
		local input = io.open(fin, "r")
		local content = input:read("*a")
		
		local name, app, author = VersionInfo.name, VersionInfo.file, VersionInfo.author
		local ma,mi,p,b = VersionInfo.major, VersionInfo.minor, VersionInfo.patch, VersionInfo.build
		
		content = content:gsub("~V_STRING~", VERSION_FORMAT:format(ma,mi,p,b))
		content = content:gsub("~V_STRING_NICE~", VERSION_NICE_FORMAT:format(ma,mi,p,b))
		content = content:gsub("~V_MAJOR~", ma)
		content = content:gsub("~V_MINOR~", mi)
		content = content:gsub("~V_PATCH~", p)
		content = content:gsub("~V_BUILD~", b)
		content = content:gsub("~V_NAME~", name)
		content = content:gsub("~V_FILE~", app)
		content = content:gsub("~V_AUTHOR~", author)
		
		local output = io.open(fout, "w")
		output:write(content)
		
	end,
	
	init = function(name, exe, author, major, minor, patch, build)
		VersionInfo.name = name
		VersionInfo.file = exe
		VersionInfo.author = author
		VersionInfo.major = major
		VersionInfo.minor = minor
		VersionInfo.patch = patch
		VersionInfo.build = build
		
		SaveVInfo()
	end
}

local cmd = ...

cmd = cmd:lower()

if not Commands[cmd] then
	print("Unknown command: " .. cmd)
else
	Commands[cmd](select(2, ...))
end
