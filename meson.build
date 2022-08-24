project('STripper', 'c', version: 'testing', subproject_dir: 'ext')

subdir('src')

executable('stripper_cli', 'src/cli.c',
    link_with: [lib_stripper, lib_dkcomp]
)

if (host_machine.system() == 'windows')
  executable('stripper_win', ['src/win.c', 'src/panel.c'],
      link_with: [lib_stripper, lib_dkcomp],
      c_args: '-DWIN32_UI',
      win_subsystem: 'windows'
  )
else
  dep_gtk3 = dependency('gtk+-3.0', required: false)
  executable('stripper_gtk', ['src/gtk.c', 'src/panel.c'],
      link_with: [lib_stripper, lib_dkcomp],
      dependencies: [dep_gtk3],
      c_args: '-DGTK3_UI'
  )
endif

