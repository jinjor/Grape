window.BENCHMARK_DATA = {
  "lastUpdate": 1627668177589,
  "repoUrl": "https://github.com/jinjor/Grape",
  "entries": {
    "Grape Benchmark": [
      {
        "commit": {
          "author": {
            "email": "jinjorweb@gmail.com",
            "name": "Yosuke Torii",
            "username": "jinjor"
          },
          "committer": {
            "email": "jinjorweb@gmail.com",
            "name": "Yosuke Torii",
            "username": "jinjor"
          },
          "distinct": true,
          "id": "09a81bba710830cdb3b0883e8a8d2e8429addae0",
          "message": "trigger ci",
          "timestamp": "2021-07-31T02:45:19+09:00",
          "tree_id": "1cbd04c9f454e0ed0f8024a9afdad485a196a09f",
          "url": "https://github.com/jinjor/Grape/commit/09a81bba710830cdb3b0883e8a8d2e8429addae0"
        },
        "date": 1627668175932,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_VoiceStep_empty",
            "value": 51.68490242750226,
            "unit": "ns/iter",
            "extra": "iterations: 13679623\ncpu: 51.62189045706887 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_whitenoise",
            "value": 52.75653890437331,
            "unit": "ns/iter",
            "extra": "iterations: 13311022\ncpu: 52.73036135016529 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_pinknoise",
            "value": 52.88029815001417,
            "unit": "ns/iter",
            "extra": "iterations: 13219520\ncpu: 52.840799060782835 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine",
            "value": 52.257008218299895,
            "unit": "ns/iter",
            "extra": "iterations: 13473717\ncpu: 52.210833877540956 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_square",
            "value": 52.80787870353154,
            "unit": "ns/iter",
            "extra": "iterations: 13336382\ncpu: 52.78837993692739 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_vibrato",
            "value": 60.924495955334464,
            "unit": "ns/iter",
            "extra": "iterations: 11684778\ncpu: 60.89623611163172 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_tremolo",
            "value": 60.976026521617605,
            "unit": "ns/iter",
            "extra": "iterations: 11682243\ncpu: 60.94548795124365 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison",
            "value": 53.23961686942308,
            "unit": "ns/iter",
            "extra": "iterations: 13515601\ncpu: 53.21812918271267 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison_with_autopan",
            "value": 61.31635092045078,
            "unit": "ns/iter",
            "extra": "iterations: 11319534\ncpu: 61.28838872695647 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_sine",
            "value": 57.49788428987936,
            "unit": "ns/iter",
            "extra": "iterations: 12370551\ncpu: 57.47124764288993 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter",
            "value": 79.08216449060276,
            "unit": "ns/iter",
            "extra": "iterations: 8884714\ncpu: 79.05578052371759 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterfreq",
            "value": 89.99149352899948,
            "unit": "ns/iter",
            "extra": "iterations: 7997441\ncpu: 89.95840044334172 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterq",
            "value": 87.90566849812052,
            "unit": "ns/iter",
            "extra": "iterations: 8013463\ncpu: 87.87936002200308 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter",
            "value": 92.26992661744434,
            "unit": "ns/iter",
            "extra": "iterations: 7543754\ncpu: 92.24478953051752 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_abs_filter",
            "value": 104.95701197457603,
            "unit": "ns/iter",
            "extra": "iterations: 6709287\ncpu: 104.9259034529304 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter",
            "value": 139.64423424271808,
            "unit": "ns/iter",
            "extra": "iterations: 5068571\ncpu: 139.5710941012766 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_full",
            "value": 189.43649225534884,
            "unit": "ns/iter",
            "extra": "iterations: 3728561\ncpu: 189.3963917983375 ns\nthreads: 1"
          }
        ]
      }
    ]
  }
}