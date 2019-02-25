--
-- icc.lua
-- icc toolset adapter for Premake
-- Copyright (c) 2013 Jason Perkins and the Premake project
--

	local p = premake
	p.tools.icc = {}
	local clang = p.tools.clang
	local gcc = p.tools.gcc
	local icc = p.tools.icc
	local config = p.config



--
-- Build a list of flags for the C preprocessor corresponding to the
-- settings in a particular project configuration.
--
-- @param cfg
--    The project configuration.
-- @return
--    An array of C preprocessor flags.
--

	function icc.getcppflags(cfg)

		-- Just pass through to GCC for now
		local flags = gcc.getcppflags(cfg)
		return flags

	end


--
-- Build a list of C compiler flags corresponding to the settings in
-- a particular project configuration. These flags are exclusive
-- of the C++ compiler flags, there is no overlap.
--
-- @param cfg
--    The project configuration.
-- @return
--    An array of C compiler flags.
--

	icc.shared = {
		architecture = gcc.shared.architecture,
		flags = gcc.shared.flags,
		floatingpoint = {
			Fast = "-fp-model fast=2",
			Strict = "-fp-model strict",
		},
		strictaliasing = gcc.shared.strictaliasing,
		optimize = {
			Off = "-O0",
			On = "-O2",
			Debug = "-O0",
			Full = "-O3",
			Size = "-O2",
			Speed = "-O2",
		},
		pic = gcc.shared.pic,
		vectorextensions = {
			AVX = "-xAVX",
			AVX2 = "-xAVX2",
			SSE = "-xSSE2",
			SSE2 = "-xSSE2",
			SSE3 = "-xSSE3",
			SSSE3 = "-xSSSE3",
			["SSE4.1"] = "-xSSE4.1",
			["HOST"] = "-xHOST",
		},
		isaextensions = gcc.shared.isaextensions,
		warnings = gcc.shared.warnings,
		symbols = gcc.shared.symbols,
		unsignedchar = gcc.shared.unsignedchar,
		omitframepointer = gcc.shared.omitframepointer
	}

	icc.cflags = table.merge(gcc.cflags, {})

	function icc.getcflags(cfg)
		local shared = config.mapFlags(cfg, icc.shared)
		local cflags = config.mapFlags(cfg, icc.cflags)

		local flags = table.join(shared, cflags)
		flags = table.join(flags, icc.getwarnings(cfg))

		return flags
	end

	function icc.getwarnings(cfg)
		return gcc.getwarnings(cfg)
	end


--
-- Build a list of C++ compiler flags corresponding to the settings
-- in a particular project configuration. These flags are exclusive
-- of the C compiler flags, there is no overlap.
--
-- @param cfg
--    The project configuration.
-- @return
--    An array of C++ compiler flags.
--

	icc.cxxflags = table.merge(gcc.cxxflags, {
	})

	function icc.getcxxflags(cfg)
		local shared = config.mapFlags(cfg, icc.shared)
		local cxxflags = config.mapFlags(cfg, icc.cxxflags)
		local flags = table.join(shared, cxxflags)
		flags = table.join(flags, icc.getwarnings(cfg))
		return flags
	end


--
-- Returns a list of defined preprocessor symbols, decorated for
-- the compiler command line.
--
-- @param defines
--    An array of preprocessor symbols to define; as an array of
--    string values.
-- @return
--    An array of symbols with the appropriate flag decorations.
--

	function icc.getdefines(defines)

		-- Just pass through to GCC for now
		local flags = gcc.getdefines(defines)
		return flags

	end

	function icc.getundefines(undefines)

		-- Just pass through to GCC for now
		local flags = gcc.getundefines(undefines)
		return flags

	end



--
-- Returns a list of forced include files, decorated for the compiler
-- command line.
--
-- @param cfg
--    The project configuration.
-- @return
--    An array of force include files with the appropriate flags.
--

	function icc.getforceincludes(cfg)

		-- Just pass through to GCC for now
		local flags = gcc.getforceincludes(cfg)
		return flags

	end


--
-- Returns a list of include file search directories, decorated for
-- the compiler command line.
--
-- @param cfg
--    The project configuration.
-- @param dirs
--    An array of include file search directories; as an array of
--    string values.
-- @return
--    An array of symbols with the appropriate flag decorations.
--

	function icc.getincludedirs(cfg, dirs, sysdirs)

		-- Just pass through to GCC for now
		local flags = gcc.getincludedirs(cfg, dirs, sysdirs)
		return flags

	end

	icc.getrunpathdirs = gcc.getrunpathdirs

--
-- get the right output flag.
--
	function icc.getsharedlibarg(cfg)
		return gcc.getsharedlibarg(cfg)
	end

--
-- Build a list of linker flags corresponding to the settings in
-- a particular project configuration.
--
-- @param cfg
--    The project configuration.
-- @return
--    An array of linker flags.
--

	icc.ldflags = {
		architecture = {
			x86 = "-m32",
			x86_64 = "-m64",
		},
		flags = {
			LinkTimeOptimization = "-flto",
		},
		kind = {
			SharedLib = function(cfg)
				local r = { icc.getsharedlibarg(cfg) }
				if cfg.system == "windows" and not cfg.flags.NoImportLib then
					table.insert(r, '-Wl,--out-implib="' .. cfg.linktarget.relpath .. '"')
				elseif cfg.system == p.LINUX then
					table.insert(r, '-Wl,-soname=' .. p.quoted(cfg.linktarget.name))
				elseif table.contains(os.getSystemTags(cfg.system), "darwin") then
					table.insert(r, '-Wl,-install_name,' .. p.quoted('@rpath/' .. cfg.linktarget.name))
				end
				return r
			end,
			WindowedApp = function(cfg)
				if cfg.system == p.WINDOWS then return "-mwindows" end
			end,
		},
		system = {
			wii = "$(MACHDEP)",
		}
	}

	function icc.getldflags(cfg)
		local flags = config.mapFlags(cfg, icc.ldflags)
		return flags
	end



--
-- Build a list of additional library directories for a particular
-- project configuration, decorated for the tool command line.
--
-- @param cfg
--    The project configuration.
-- @return
--    An array of decorated additional library directories.
--

	function icc.getLibraryDirectories(cfg)

		-- Just pass through to GCC for now
		local flags = gcc.getLibraryDirectories(cfg)
		return flags

	end


--
-- Build a list of libraries to be linked for a particular project
-- configuration, decorated for the linker command line.
--
-- @param cfg
--    The project configuration.
-- @param systemOnly
--    Boolean flag indicating whether to link only system libraries,
--    or system libraries and sibling projects as well.
-- @return
--    A list of libraries to link, decorated for the linker.
--

	function icc.getlinks(cfg, systemonly, nogroups)
		return gcc.getlinks(cfg, systemonly, nogroups)
	end


--
-- Return a list of makefile-specific configuration rules. This will
-- be going away when I get a chance to overhaul these adapters.
--
-- @param cfg
--    The project configuration.
-- @return
--    A list of additional makefile rules.
--

	function icc.getmakesettings(cfg)

		-- Just pass through to GCC for now
		local flags = gcc.getmakesettings(cfg)
		return flags

	end


--
-- Retrieves the executable command name for a tool, based on the
-- provided configuration and the operating environment. I will
-- be moving these into global configuration blocks when I get
-- the chance.
--
-- @param cfg
--    The configuration to query.
-- @param tool
--    The tool to fetch, one of "cc" for the C compiler, "cxx" for
--    the C++ compiler, or "ar" for the static linker.
-- @return
--    The executable command name for a tool, or nil if the system's
--    default value should be used.
--

	icc.tools = {
		cc = "icc",
		cxx = "icpc",
		ar = "ar"
	}

	function icc.gettoolname(cfg, tool)
		return icc.tools[tool]
	end
