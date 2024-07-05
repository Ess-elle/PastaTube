#include <failingplugin/PluginProcessor.h>
#include <gtest/gtest.h>

namespace audio_plugin_test {
TEST(AudioPluginAudioProcessor, Foo) {
  AudioPluginAudioProcessor processor{};
  ASSERT_FALSE(true);
}
}  // namespace audio_plugin_test