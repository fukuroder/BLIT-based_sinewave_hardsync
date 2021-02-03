
set(blit_based_sinewave_hardsync_sources
    source/BLITSineHardSync_processor.cpp
    source/BLITSineHardSync_processor.h
    source/BLITSineHardSync_controller.cpp
    source/BLITSineHardSync_controller.h
    source/BLITSineHardSync_entry.cpp
    source/BLITSineHardSync_guids.h
    source/BLITSineHardSync_oscillator.cpp
    source/BLITSineHardSync_oscillator.h
    remez_approx/remez_approx.h
)

set(target blit_based_sinewave_hardsync)
smtg_add_vst3plugin(${target} ${blit_based_sinewave_hardsync_sources})
set_target_properties(${target} PROPERTIES ${SDK_IDE_PLUGIN_EXAMPLES_FOLDER})
target_link_libraries(${target} PRIVATE sdk)
