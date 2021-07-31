window.BENCHMARK_DATA = {
  "lastUpdate": 1627703088015,
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
      },
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
          "id": "0b50f9deefd1ae2559a75fffca145a4b2dbd0a08",
          "message": "refactor pitch bend",
          "timestamp": "2021-07-31T11:44:18+09:00",
          "tree_id": "edcf8f4d90d98111ce791ad30f0660e454d8c343",
          "url": "https://github.com/jinjor/Grape/commit/0b50f9deefd1ae2559a75fffca145a4b2dbd0a08"
        },
        "date": 1627700475057,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_VoiceStep_empty",
            "value": 51.25359133090939,
            "unit": "ns/iter",
            "extra": "iterations: 13713927\ncpu: 51.10950349961758 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_whitenoise",
            "value": 52.05836470141611,
            "unit": "ns/iter",
            "extra": "iterations: 13310768\ncpu: 52.047935926762456 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_pinknoise",
            "value": 52.02713235596652,
            "unit": "ns/iter",
            "extra": "iterations: 13487476\ncpu: 52.012400244493485 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine",
            "value": 52.1732046522322,
            "unit": "ns/iter",
            "extra": "iterations: 13454552\ncpu: 52.15550841083376 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_square",
            "value": 52.01511880276991,
            "unit": "ns/iter",
            "extra": "iterations: 13374604\ncpu: 51.9934646289341 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_vibrato",
            "value": 59.57952718212619,
            "unit": "ns/iter",
            "extra": "iterations: 11660642\ncpu: 59.56464489691043 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_tremolo",
            "value": 59.5763583447252,
            "unit": "ns/iter",
            "extra": "iterations: 11735515\ncpu: 59.553415423183345 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison",
            "value": 51.95929487402853,
            "unit": "ns/iter",
            "extra": "iterations: 13443442\ncpu: 51.94644347779378 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison_with_autopan",
            "value": 59.917587529853336,
            "unit": "ns/iter",
            "extra": "iterations: 11767673\ncpu: 59.899013169383686 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_sine",
            "value": 58.133024275025925,
            "unit": "ns/iter",
            "extra": "iterations: 11999246\ncpu: 58.113568135864554 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter",
            "value": 79.43273246456874,
            "unit": "ns/iter",
            "extra": "iterations: 8844638\ncpu: 79.40324974295152 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterfreq",
            "value": 86.19546612994237,
            "unit": "ns/iter",
            "extra": "iterations: 8124891\ncpu: 86.16164819934174 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterq",
            "value": 86.08863765832083,
            "unit": "ns/iter",
            "extra": "iterations: 8206715\ncpu: 86.05574825006101 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter",
            "value": 87.17156186416243,
            "unit": "ns/iter",
            "extra": "iterations: 7975572\ncpu: 87.15211899535232 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_abs_filter",
            "value": 102.97823408661493,
            "unit": "ns/iter",
            "extra": "iterations: 6793742\ncpu: 102.95990044956089 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter",
            "value": 127.01778179574124,
            "unit": "ns/iter",
            "extra": "iterations: 5516372\ncpu: 126.96714434777059 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_full",
            "value": 185.81668980491335,
            "unit": "ns/iter",
            "extra": "iterations: 3718953\ncpu: 185.7802451388873 ns\nthreads: 1"
          }
        ]
      },
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
          "id": "6b615cae3630fddad4d579fdaf3a74765755bb77",
          "message": "make Modifiers a simple struct",
          "timestamp": "2021-07-31T12:28:23+09:00",
          "tree_id": "632bf4eeec9e3f76ffbb92681e2e60e73a97c509",
          "url": "https://github.com/jinjor/Grape/commit/6b615cae3630fddad4d579fdaf3a74765755bb77"
        },
        "date": 1627703086930,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_VoiceStep_empty",
            "value": 33.427824040672796,
            "unit": "ns/iter",
            "extra": "iterations: 20634724\ncpu: 33.417456904197024 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_whitenoise",
            "value": 37.66589221542176,
            "unit": "ns/iter",
            "extra": "iterations: 18422507\ncpu: 37.64959893895007 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_pinknoise",
            "value": 37.82225488105847,
            "unit": "ns/iter",
            "extra": "iterations: 18525527\ncpu: 37.811016118461865 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine",
            "value": 37.812544730471735,
            "unit": "ns/iter",
            "extra": "iterations: 19048811\ncpu: 37.77962834530721 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_square",
            "value": 37.26619337332532,
            "unit": "ns/iter",
            "extra": "iterations: 18634814\ncpu: 37.25317569577031 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_vibrato",
            "value": 49.04609935458025,
            "unit": "ns/iter",
            "extra": "iterations: 14615671\ncpu: 49.04064958769256 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_tremolo",
            "value": 49.479876602005746,
            "unit": "ns/iter",
            "extra": "iterations: 13906548\ncpu: 49.46489955666933 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison",
            "value": 37.138853706283115,
            "unit": "ns/iter",
            "extra": "iterations: 18560304\ncpu: 37.122991088939074 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison_with_autopan",
            "value": 49.26806096419158,
            "unit": "ns/iter",
            "extra": "iterations: 13859739\ncpu: 49.249556575343924 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_sine",
            "value": 46.51441301116462,
            "unit": "ns/iter",
            "extra": "iterations: 15426929\ncpu: 46.48462438635723 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter",
            "value": 61.48258713088131,
            "unit": "ns/iter",
            "extra": "iterations: 11797223\ncpu: 61.463278264723726 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterfreq",
            "value": 69.60824005733134,
            "unit": "ns/iter",
            "extra": "iterations: 10196798\ncpu: 69.58988498154021 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterq",
            "value": 66.59572550680424,
            "unit": "ns/iter",
            "extra": "iterations: 10040161\ncpu: 66.56915163013817 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter",
            "value": 75.22970919927764,
            "unit": "ns/iter",
            "extra": "iterations: 9700800\ncpu: 75.09421903348179 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_abs_filter",
            "value": 83.59785326928022,
            "unit": "ns/iter",
            "extra": "iterations: 8559341\ncpu: 83.56612968218012 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter",
            "value": 111.83938059720045,
            "unit": "ns/iter",
            "extra": "iterations: 6290834\ncpu: 111.65212752394959 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_full",
            "value": 176.48058928706374,
            "unit": "ns/iter",
            "extra": "iterations: 3890532\ncpu: 175.29247928046823 ns\nthreads: 1"
          }
        ]
      }
    ]
  }
}