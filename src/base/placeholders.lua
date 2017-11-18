--
-- placeholders.lua
-- Copyright (c) 2017
--

local p = premake

function global(name)
	printf("global '%s'", name)
end

function workspace(name)
	printf("workspace '%s'", name)
end

function solution(name)
	printf("solution '%s'", name)
end

function project(name)
	printf("project '%s'", name)
end

function group(name)
	printf("group '%s'", name)
end

function configuration(terms)
	printf("configuration " .. premake.valueToCode(terms))
end

function filter(terms)
	print("filter " .. premake.valueToCode(terms))
end



-- Blizzard extensions...

	bnet = {}
	cache = {}
	p.packagemanager = {}

	p.api.register {
		name = 'use_exposed',
		scope = 'config',
		kind = 'list:string'
	}

	p.api.register {
		name = 'use_private',
		scope = 'config',
		kind = 'list:string'
	}

	p.api.register {
		name = 'includedependencies',
		scope = 'config',
		kind = 'tableorstring'
	}

	p.api.register {
		name = 'linkdependencies',
		scope = 'config',
		kind = 'tableorstring'
	}

	p.api.register {
		name = 'bindirdependencies',
		scope = 'config',
		kind = 'tableorstring'
	}

	p.api.register {
		name = 'copybindependencies',
		scope = 'config',
		kind = 'tableorstring',
	}

	p.api.register {
		name = 'copybintarget',
		scope = 'config',
		kind = 'path',
		tokens = true,
		pathVars = true,
	}

	p.api.register {
		name  = 'lockfile',
		scope = 'global',
		kind  = 'file',
	}

	p.api.register {
		name     = 'package_location',
		scope    = 'project',
		kind     = 'path',
		tokens   = true,
		pathVars = true,
	}

	p.api.register {
		name     = 'package_buildlog',
		scope    = 'config',
		kind     = 'path',
		tokens   = true,
		pathVars = true,
	}

	p.api.register {
		name     = 'package_objdir',
		scope    = 'config',
		kind     = 'path',
		tokens   = true,
		pathVars = true,
	}

	p.api.register {
		name     = 'package_libdir', -- this controls the targetdir for StaticLib projects.
		scope    = 'config',
		kind     = 'path',
		tokens   = true,
		pathVars = true,
	}

	p.api.register {
		name     = 'package_bindir', -- this controls the targetdir for SharedLib, WindowedApp and ConsoleApp projects.
		scope    = 'config',
		kind     = 'path',
		tokens   = true,
		pathVars = true,
	}

	-- initialize sane defaults.
	package_location (path.join(_MAIN_SCRIPT_DIR, _OPTIONS.to, 'projects/packages'))
	package_bindir   (path.join(_MAIN_SCRIPT_DIR, "bin/%{premake.packagemanager.buildVariantFromConfig(cfg)}"))
	package_objdir   (path.join(_MAIN_SCRIPT_DIR, _OPTIONS.to, '%{premake.packagemanager.buildVariantFromConfig(cfg)}', 'obj'))
	package_libdir   (path.join(_MAIN_SCRIPT_DIR, _OPTIONS.to, '%{premake.packagemanager.buildVariantFromConfig(cfg)}', 'lib'))

---
-- shortcut if you need both include & link dependencies
---
	function usedependencies(table)
		includedependencies(table)
		linkdependencies(table)
	end

	function cache.use_env_var_location()
	end

	function import(tbl)
		print("import " .. premake.valueToCode(tbl))
	end



-- XCode extensions...

	p.api.register {
		name = "xcode_settings",
		scope = "config",
		kind = "keyed:mixed",
		tokens = true
	}

	p.api.register {
		name = "xcode_filesettings",
		scope = "config",
		kind = "keyed:mixed",
		tokens = true
	}

	p.api.register {
		name = "xcode_filetype",
		scope = "project",
		kind = "string",
		tokens = true
	}

	p.api.register {
		name = "xcode_resources",
		scope = "project",
		kind = "list:file",
		tokens = true
	}

	p.api.register {
		name = "xcode_weaklinks",
		scope = "config",
		kind = "list:mixed",
		tokens = true
	}

	p.api.register {
		name = "xcode_frameworkdirs",
		scope = "config",
		kind = "list:directory",
		tokens = true
	}

	p.api.register {
		name = "xcode_runpathdirs",
		scope = "config",
		kind = "list:string",
		tokens = true
	}

	p.api.register {
		name = "xcode_targetattributes",
		scope = "project",
		kind  = "keyed:string"
	}


-- Android extensions...

	p.api.register {
		name = "floatabi",
		scope = "config",
		kind = "string",
		allowed = {
			"soft",
			"softfp",
			"hard",
		},
	}

	p.api.register {
		name = "androidapilevel",
		scope = "config",
		kind = "integer",
	}

	p.api.register {
		name = "toolchainversion",
		scope = "config",
		kind = "string",
		allowed = {
			"4.6", -- NDK GCC versions
			"4.8",
			"4.9",
			"3.4", -- NDK clang versions
			"3.5",
			"3.6",
			"3.8",
		},
	}

	p.api.register {
		name = "stl",
		scope = "config",
		kind = "string",
		allowed = {
			"none",
			"minimal c++ (system)",
			"c++ static",
			"c++ shared",
			"stlport static",
			"stlport shared",
			"gnu stl static",
			"gnu stl shared",
			"llvm libc++ static",
			"llvm libc++ shared",
		},
	}

	p.api.register {
		name = "thumbmode",
		scope = "config",
		kind = "string",
		allowed = {
			"thumb",
			"arm",
			"disabled",
		},
	}

	p.api.register {
		name = "androidapplibname",
		scope = "config",
		kind = "string"
	}


-- Console extensions...

	p.api.register {
		name = "compileaswinrt",
		scope = "config",
		kind = "boolean"
	}

	p.api.register {
		name = "dummyappxmanifest",
		scope = "config",
		kind = "boolean"
	}
