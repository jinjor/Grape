window.BENCHMARK_DATA = {
  "lastUpdate": 1629530029329,
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
          "id": "e73c5b90b7eb7b93e3e95d61fdbd7074ff56842f",
          "message": "add test case",
          "timestamp": "2021-08-01T15:45:54+09:00",
          "tree_id": "94764138b9ae027e39669ca78c02f1e75cbb1c90",
          "url": "https://github.com/jinjor/Grape/commit/e73c5b90b7eb7b93e3e95d61fdbd7074ff56842f"
        },
        "date": 1627801474812,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_VoiceStep_empty",
            "value": 33.641014553361025,
            "unit": "ns/iter",
            "extra": "iterations: 20766892\ncpu: 33.6104218195 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_whitenoise",
            "value": 37.94843684407356,
            "unit": "ns/iter",
            "extra": "iterations: 18811591\ncpu: 37.92709505538369 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_pinknoise",
            "value": 37.6857692133942,
            "unit": "ns/iter",
            "extra": "iterations: 18874691\ncpu: 37.643848050280695 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine",
            "value": 37.462905517204646,
            "unit": "ns/iter",
            "extra": "iterations: 18949597\ncpu: 36.939255225322235 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_square",
            "value": 37.23517069472361,
            "unit": "ns/iter",
            "extra": "iterations: 18754739\ncpu: 37.18489497507802 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_vibrato",
            "value": 48.93803752669922,
            "unit": "ns/iter",
            "extra": "iterations: 13960353\ncpu: 48.891528745727264 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_tremolo",
            "value": 49.91058353601674,
            "unit": "ns/iter",
            "extra": "iterations: 14177445\ncpu: 49.16894405162568 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison",
            "value": 37.822938684753666,
            "unit": "ns/iter",
            "extra": "iterations: 18834419\ncpu: 37.74669131020184 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison_with_autopan",
            "value": 49.33059934826053,
            "unit": "ns/iter",
            "extra": "iterations: 13783055\ncpu: 49.28958057556894 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_sine",
            "value": 45.86678046340086,
            "unit": "ns/iter",
            "extra": "iterations: 15057844\ncpu: 45.18103654148631 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter",
            "value": 59.68759337379001,
            "unit": "ns/iter",
            "extra": "iterations: 11658117\ncpu: 59.64222180992007 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterfreq",
            "value": 68.95036026921217,
            "unit": "ns/iter",
            "extra": "iterations: 10223605\ncpu: 68.84147030328347 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterq",
            "value": 67.97931096454668,
            "unit": "ns/iter",
            "extra": "iterations: 10215846\ncpu: 67.92995900682126 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter",
            "value": 74.35727002006358,
            "unit": "ns/iter",
            "extra": "iterations: 9632188\ncpu: 73.35986382325598 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter_with_vibrato",
            "value": 84.63064139153745,
            "unit": "ns/iter",
            "extra": "iterations: 8302495\ncpu: 84.55265555715476 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_abs_filter",
            "value": 88.83563589984551,
            "unit": "ns/iter",
            "extra": "iterations: 7979936\ncpu: 88.76086224250425 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter",
            "value": 111.05937491062298,
            "unit": "ns/iter",
            "extra": "iterations: 6328599\ncpu: 110.94604034795061 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter_with_vibrato",
            "value": 124.94689368640516,
            "unit": "ns/iter",
            "extra": "iterations: 5694483\ncpu: 123.70552339869961 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_full",
            "value": 170.8410242939101,
            "unit": "ns/iter",
            "extra": "iterations: 4153144\ncpu: 170.32542093411547 ns\nthreads: 1"
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
          "id": "df7d278be2769903e840ec411d10bb95a22e0613",
          "message": "fix benchmark logic",
          "timestamp": "2021-08-01T23:59:49+09:00",
          "tree_id": "e5b6c7b4be4dcb215cca8c3e43053468fc0a78bf",
          "url": "https://github.com/jinjor/Grape/commit/df7d278be2769903e840ec411d10bb95a22e0613"
        },
        "date": 1627831026995,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_VoiceStep_empty",
            "value": 33.92696387296939,
            "unit": "ns/iter",
            "extra": "iterations: 20041342\ncpu: 33.90451597502801 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_whitenoise",
            "value": 70.36378760967172,
            "unit": "ns/iter",
            "extra": "iterations: 9948269\ncpu: 70.28408660843411 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_pinknoise",
            "value": 76.7801937061502,
            "unit": "ns/iter",
            "extra": "iterations: 9117314\ncpu: 76.70899565376384 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine",
            "value": 81.45678256555688,
            "unit": "ns/iter",
            "extra": "iterations: 8585586\ncpu: 81.38943573566209 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_square",
            "value": 90.1978202472055,
            "unit": "ns/iter",
            "extra": "iterations: 7765009\ncpu: 90.15456389039608 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_vibrato",
            "value": 149.90785670139286,
            "unit": "ns/iter",
            "extra": "iterations: 4672928\ncpu: 149.81805839935888 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_tremolo",
            "value": 145.06241282711184,
            "unit": "ns/iter",
            "extra": "iterations: 4811559\ncpu: 144.9501086861867 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison",
            "value": 122.92450270621038,
            "unit": "ns/iter",
            "extra": "iterations: 5698191\ncpu: 122.61417702565593 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison_with_autopan",
            "value": 232.07958291356925,
            "unit": "ns/iter",
            "extra": "iterations: 3018751\ncpu: 231.6978114458595 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_sine",
            "value": 169.02329731331884,
            "unit": "ns/iter",
            "extra": "iterations: 4153612\ncpu: 168.92068878845708 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter",
            "value": 103.75590355960666,
            "unit": "ns/iter",
            "extra": "iterations: 6767993\ncpu: 103.52625955730166 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterfreq",
            "value": 157.41059804973779,
            "unit": "ns/iter",
            "extra": "iterations: 4481032\ncpu: 157.03837865920184 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterq",
            "value": 121.47306587962882,
            "unit": "ns/iter",
            "extra": "iterations: 5830467\ncpu: 121.309836759217 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter",
            "value": 113.21821712406059,
            "unit": "ns/iter",
            "extra": "iterations: 6197268\ncpu: 113.11258444850213 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter_with_vibrato",
            "value": 140.93382996839404,
            "unit": "ns/iter",
            "extra": "iterations: 4974276\ncpu: 140.86311254140304 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_abs_filter",
            "value": 129.2877066009475,
            "unit": "ns/iter",
            "extra": "iterations: 5447216\ncpu: 129.20545100469684 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter",
            "value": 152.7587091709361,
            "unit": "ns/iter",
            "extra": "iterations: 4595558\ncpu: 152.61715769880362 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter_with_vibrato",
            "value": 177.06187021291703,
            "unit": "ns/iter",
            "extra": "iterations: 3940022\ncpu: 176.9177938600344 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_full",
            "value": 469.9952544059019,
            "unit": "ns/iter",
            "extra": "iterations: 1491278\ncpu: 469.7715650603036 ns\nthreads: 1"
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
          "id": "5e60f71842c4f3a5be2ef0607553d4820518907c",
          "message": "make autopan a bit faster",
          "timestamp": "2021-08-03T15:49:31+09:00",
          "tree_id": "867704b9ca5e748658ee682affbc761426a8347a",
          "url": "https://github.com/jinjor/Grape/commit/5e60f71842c4f3a5be2ef0607553d4820518907c"
        },
        "date": 1627974377595,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_VoiceStep_empty",
            "value": 34.79126341954795,
            "unit": "ns/iter",
            "extra": "iterations: 20205869\ncpu: 34.77558921123363 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_whitenoise",
            "value": 71.4252167548648,
            "unit": "ns/iter",
            "extra": "iterations: 9859294\ncpu: 71.3928400958527 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_pinknoise",
            "value": 77.69795450008428,
            "unit": "ns/iter",
            "extra": "iterations: 9010169\ncpu: 77.6769003999814 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine",
            "value": 82.10784525272643,
            "unit": "ns/iter",
            "extra": "iterations: 8429977\ncpu: 82.09619077252523 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_square",
            "value": 92.20287265574184,
            "unit": "ns/iter",
            "extra": "iterations: 7595898\ncpu: 92.17698815860876 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_vibrato",
            "value": 152.19218304990972,
            "unit": "ns/iter",
            "extra": "iterations: 4569071\ncpu: 152.089779300869 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_tremolo",
            "value": 146.45061346947838,
            "unit": "ns/iter",
            "extra": "iterations: 4798201\ncpu: 146.38986570174933 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison",
            "value": 131.307910747719,
            "unit": "ns/iter",
            "extra": "iterations: 5322708\ncpu: 131.2431566788935 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison_with_autopan",
            "value": 218.35464708060516,
            "unit": "ns/iter",
            "extra": "iterations: 3234407\ncpu: 218.23351235636108 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_sine",
            "value": 170.7193796222566,
            "unit": "ns/iter",
            "extra": "iterations: 4094022\ncpu: 170.63318174645863 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter",
            "value": 105.78636057265952,
            "unit": "ns/iter",
            "extra": "iterations: 6660133\ncpu: 105.76860852478443 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterfreq",
            "value": 160.41357168285708,
            "unit": "ns/iter",
            "extra": "iterations: 4362893\ncpu: 160.35277509670732 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterq",
            "value": 122.6736524981438,
            "unit": "ns/iter",
            "extra": "iterations: 5714519\ncpu: 122.60541963374368 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter",
            "value": 114.38725595530433,
            "unit": "ns/iter",
            "extra": "iterations: 6124288\ncpu: 114.36774364628158 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter_with_vibrato",
            "value": 141.4983179861849,
            "unit": "ns/iter",
            "extra": "iterations: 4960423\ncpu: 141.45507348869242 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_abs_filter",
            "value": 130.81086534492175,
            "unit": "ns/iter",
            "extra": "iterations: 5363861\ncpu: 130.68944180320864 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter",
            "value": 154.56473841500477,
            "unit": "ns/iter",
            "extra": "iterations: 4500450\ncpu: 154.52143674521432 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter_with_vibrato",
            "value": 183.21709223839622,
            "unit": "ns/iter",
            "extra": "iterations: 3858535\ncpu: 183.14541658945697 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_full",
            "value": 486.21845475572104,
            "unit": "ns/iter",
            "extra": "iterations: 1440756\ncpu: 485.8143918887015 ns\nthreads: 1"
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
          "id": "2a9b863200c170e9460f8c84bf84399c17452e34",
          "message": "make autopan a bit faster",
          "timestamp": "2021-08-03T16:52:14+09:00",
          "tree_id": "a1a1f7acdd122ed898d052e37464eaf679102b2b",
          "url": "https://github.com/jinjor/Grape/commit/2a9b863200c170e9460f8c84bf84399c17452e34"
        },
        "date": 1627978138590,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_VoiceStep_empty",
            "value": 35.94920866147346,
            "unit": "ns/iter",
            "extra": "iterations: 19027634\ncpu: 35.93552409090904 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_whitenoise",
            "value": 71.94140178606983,
            "unit": "ns/iter",
            "extra": "iterations: 9415816\ncpu: 71.93927748800529 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_pinknoise",
            "value": 78.36485549685318,
            "unit": "ns/iter",
            "extra": "iterations: 8913337\ncpu: 78.34192738364997 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine",
            "value": 83.10847572106765,
            "unit": "ns/iter",
            "extra": "iterations: 8362203\ncpu: 83.08217344161581 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_square",
            "value": 91.68461051938932,
            "unit": "ns/iter",
            "extra": "iterations: 7576578\ncpu: 91.64955471982209 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_vibrato",
            "value": 150.7244424317354,
            "unit": "ns/iter",
            "extra": "iterations: 4679113\ncpu: 150.68475585009386 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_tremolo",
            "value": 146.5192819735164,
            "unit": "ns/iter",
            "extra": "iterations: 4843721\ncpu: 146.44877357717357 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison",
            "value": 131.06778289320906,
            "unit": "ns/iter",
            "extra": "iterations: 5323113\ncpu: 131.04700200803552 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison_with_autopan",
            "value": 206.97027738919493,
            "unit": "ns/iter",
            "extra": "iterations: 3352330\ncpu: 206.90027533088943 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_sine",
            "value": 168.96316325110365,
            "unit": "ns/iter",
            "extra": "iterations: 4160302\ncpu: 168.92667888052392 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter",
            "value": 106.5208139102288,
            "unit": "ns/iter",
            "extra": "iterations: 6607264\ncpu: 106.48401516875981 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterfreq",
            "value": 158.91101845792755,
            "unit": "ns/iter",
            "extra": "iterations: 4468275\ncpu: 158.87741913825815 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterq",
            "value": 122.33736579903987,
            "unit": "ns/iter",
            "extra": "iterations: 5775292\ncpu: 122.30238748101391 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter",
            "value": 113.88172184947656,
            "unit": "ns/iter",
            "extra": "iterations: 6166532\ncpu: 113.8545944462787 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter_with_vibrato",
            "value": 137.83463821569586,
            "unit": "ns/iter",
            "extra": "iterations: 5081301\ncpu: 137.8054557287593 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_abs_filter",
            "value": 129.74101300097237,
            "unit": "ns/iter",
            "extra": "iterations: 5316402\ncpu: 129.71498393086154 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter",
            "value": 151.23952664092693,
            "unit": "ns/iter",
            "extra": "iterations: 4633184\ncpu: 151.18264243336768 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter_with_vibrato",
            "value": 176.15103244518485,
            "unit": "ns/iter",
            "extra": "iterations: 3986991\ncpu: 176.07187977098485 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_full",
            "value": 467.2048424328129,
            "unit": "ns/iter",
            "extra": "iterations: 1506309\ncpu: 466.97789099049476 ns\nthreads: 1"
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
          "id": "4027a2329f9268386f1e40ac529737205eaaa152",
          "message": "faster loops",
          "timestamp": "2021-08-03T23:17:07+09:00",
          "tree_id": "8944414c385a4716c49625cc573263d62b2b5bb2",
          "url": "https://github.com/jinjor/Grape/commit/4027a2329f9268386f1e40ac529737205eaaa152"
        },
        "date": 1628001332089,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_VoiceStep_empty",
            "value": 36.49610520190653,
            "unit": "ns/iter",
            "extra": "iterations: 19379952\ncpu: 36.33786089872668 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_whitenoise",
            "value": 76.92094662671927,
            "unit": "ns/iter",
            "extra": "iterations: 9176838\ncpu: 76.85915344697162 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_pinknoise",
            "value": 83.932211154788,
            "unit": "ns/iter",
            "extra": "iterations: 8584849\ncpu: 83.84748526153462 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine",
            "value": 87.22720553126999,
            "unit": "ns/iter",
            "extra": "iterations: 7893461\ncpu: 87.18697666334201 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_square",
            "value": 97.20522293660426,
            "unit": "ns/iter",
            "extra": "iterations: 7197943\ncpu: 96.94908670435427 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_vibrato",
            "value": 153.69414924680387,
            "unit": "ns/iter",
            "extra": "iterations: 4434506\ncpu: 153.56096034146753 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_tremolo",
            "value": 150.52894367603278,
            "unit": "ns/iter",
            "extra": "iterations: 4697451\ncpu: 150.42051529648734 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison",
            "value": 140.3205445356418,
            "unit": "ns/iter",
            "extra": "iterations: 5128994\ncpu: 139.43124129215204 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison_with_autopan",
            "value": 220.6950009315465,
            "unit": "ns/iter",
            "extra": "iterations: 3188354\ncpu: 220.63202517662708 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_sine",
            "value": 177.3238099207847,
            "unit": "ns/iter",
            "extra": "iterations: 4005952\ncpu: 177.15838831818252 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter",
            "value": 110.87737624089108,
            "unit": "ns/iter",
            "extra": "iterations: 6352260\ncpu: 110.86085896987842 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterfreq",
            "value": 164.78455903223514,
            "unit": "ns/iter",
            "extra": "iterations: 4368120\ncpu: 163.7455014972118 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterq",
            "value": 127.62971659365245,
            "unit": "ns/iter",
            "extra": "iterations: 5313819\ncpu: 127.55684753281989 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter",
            "value": 119.45614892366967,
            "unit": "ns/iter",
            "extra": "iterations: 5830370\ncpu: 119.395510061969 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter_with_vibrato",
            "value": 142.4532274810843,
            "unit": "ns/iter",
            "extra": "iterations: 4943747\ncpu: 142.42638225621167 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_abs_filter",
            "value": 139.3528586393229,
            "unit": "ns/iter",
            "extra": "iterations: 5106433\ncpu: 139.30820202673803 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter",
            "value": 156.64092279224744,
            "unit": "ns/iter",
            "extra": "iterations: 4480315\ncpu: 156.55751883517095 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter_with_vibrato",
            "value": 180.37706790655503,
            "unit": "ns/iter",
            "extra": "iterations: 3886902\ncpu: 180.22193510410085 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_full",
            "value": 475.72345802050074,
            "unit": "ns/iter",
            "extra": "iterations: 1468956\ncpu: 475.45331514354325 ns\nthreads: 1"
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
          "id": "2a9b863200c170e9460f8c84bf84399c17452e34",
          "message": "make autopan a bit faster",
          "timestamp": "2021-08-03T16:52:14+09:00",
          "tree_id": "a1a1f7acdd122ed898d052e37464eaf679102b2b",
          "url": "https://github.com/jinjor/Grape/commit/2a9b863200c170e9460f8c84bf84399c17452e34"
        },
        "date": 1628003563008,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_VoiceStep_empty",
            "value": 37.260908035750994,
            "unit": "ns/iter",
            "extra": "iterations: 18539314\ncpu: 37.2422086383563 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_whitenoise",
            "value": 75.26907903194007,
            "unit": "ns/iter",
            "extra": "iterations: 9413030\ncpu: 75.22508692737618 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_pinknoise",
            "value": 81.75465906332573,
            "unit": "ns/iter",
            "extra": "iterations: 8617612\ncpu: 81.7168375647453 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine",
            "value": 86.2744335376507,
            "unit": "ns/iter",
            "extra": "iterations: 7930933\ncpu: 86.23109033956028 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_square",
            "value": 94.43500598691566,
            "unit": "ns/iter",
            "extra": "iterations: 7384434\ncpu: 94.40899600429772 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_vibrato",
            "value": 153.8021968689241,
            "unit": "ns/iter",
            "extra": "iterations: 4556758\ncpu: 153.76041475101363 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_tremolo",
            "value": 149.22232346933882,
            "unit": "ns/iter",
            "extra": "iterations: 4660763\ncpu: 149.15819577180804 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison",
            "value": 135.22983353302365,
            "unit": "ns/iter",
            "extra": "iterations: 5182229\ncpu: 135.18391410337122 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison_with_autopan",
            "value": 214.69793038171144,
            "unit": "ns/iter",
            "extra": "iterations: 3256011\ncpu: 214.6362527645022 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_sine",
            "value": 174.24423649177362,
            "unit": "ns/iter",
            "extra": "iterations: 4021856\ncpu: 174.16958737458583 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter",
            "value": 108.8172390862135,
            "unit": "ns/iter",
            "extra": "iterations: 6378946\ncpu: 108.76875270616809 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterfreq",
            "value": 162.30693322609173,
            "unit": "ns/iter",
            "extra": "iterations: 4307533\ncpu: 162.2627151086253 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterq",
            "value": 127.67499192115119,
            "unit": "ns/iter",
            "extra": "iterations: 5520592\ncpu: 127.6431947878056 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter",
            "value": 117.77200895248819,
            "unit": "ns/iter",
            "extra": "iterations: 5992073\ncpu: 117.72703703709892 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter_with_vibrato",
            "value": 143.56696034579687,
            "unit": "ns/iter",
            "extra": "iterations: 4857569\ncpu: 143.4977043043545 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_abs_filter",
            "value": 134.19972384432162,
            "unit": "ns/iter",
            "extra": "iterations: 5147097\ncpu: 134.10413675903158 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter",
            "value": 156.77135078726053,
            "unit": "ns/iter",
            "extra": "iterations: 4467306\ncpu: 156.72331378240017 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter_with_vibrato",
            "value": 183.1052528864436,
            "unit": "ns/iter",
            "extra": "iterations: 3822546\ncpu: 183.03638465044014 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_full",
            "value": 481.80678656280554,
            "unit": "ns/iter",
            "extra": "iterations: 1391927\ncpu: 481.6014058208497 ns\nthreads: 1"
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
          "id": "c433ae9fee6d8951f743547ac9bf9659b24d224d",
          "message": "add benchmark case",
          "timestamp": "2021-08-04T00:36:50+09:00",
          "tree_id": "7097e0f1a80cdd5ad8706380a7576899cb7d87a4",
          "url": "https://github.com/jinjor/Grape/commit/c433ae9fee6d8951f743547ac9bf9659b24d224d"
        },
        "date": 1628008873896,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_VoiceStep_empty",
            "value": 34.90186679064743,
            "unit": "ns/iter",
            "extra": "iterations: 18650404\ncpu: 34.88047765614086 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_whitenoise",
            "value": 73.8899821198725,
            "unit": "ns/iter",
            "extra": "iterations: 9381366\ncpu: 73.48471427295345 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_pinknoise",
            "value": 79.32192749923374,
            "unit": "ns/iter",
            "extra": "iterations: 8752626\ncpu: 79.29494531127004 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine",
            "value": 79.63923642419351,
            "unit": "ns/iter",
            "extra": "iterations: 8666906\ncpu: 79.59841724370845 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_square",
            "value": 88.8240832523815,
            "unit": "ns/iter",
            "extra": "iterations: 8027155\ncpu: 88.78413335733524 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_vibrato",
            "value": 137.5980502810041,
            "unit": "ns/iter",
            "extra": "iterations: 5117558\ncpu: 136.88774997762619 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_tremolo",
            "value": 136.7016983757799,
            "unit": "ns/iter",
            "extra": "iterations: 4990945\ncpu: 136.66009142557178 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison",
            "value": 116.64363864512625,
            "unit": "ns/iter",
            "extra": "iterations: 5992945\ncpu: 116.61511994520224 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison_with_autopan",
            "value": 200.98021531009542,
            "unit": "ns/iter",
            "extra": "iterations: 3445543\ncpu: 200.9067946619734 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_sine",
            "value": 161.27506196139228,
            "unit": "ns/iter",
            "extra": "iterations: 4289768\ncpu: 161.2373909265024 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter",
            "value": 105.01492842777373,
            "unit": "ns/iter",
            "extra": "iterations: 6781960\ncpu: 104.95623684008761 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterfreq",
            "value": 154.1149806910066,
            "unit": "ns/iter",
            "extra": "iterations: 4557974\ncpu: 154.0498037066471 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_lowshelf_with_lfo_filterfreq",
            "value": 159.64664413465238,
            "unit": "ns/iter",
            "extra": "iterations: 4419471\ncpu: 158.66107052178904 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterq",
            "value": 119.9269862191794,
            "unit": "ns/iter",
            "extra": "iterations: 5864660\ncpu: 119.87839022210984 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter",
            "value": 113.90414206480578,
            "unit": "ns/iter",
            "extra": "iterations: 6142830\ncpu: 113.88838694868635 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter_with_vibrato",
            "value": 134.02834125294837,
            "unit": "ns/iter",
            "extra": "iterations: 5247298\ncpu: 133.96609073850945 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_abs_filter",
            "value": 128.60955539321165,
            "unit": "ns/iter",
            "extra": "iterations: 5466358\ncpu: 128.2460826751559 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter",
            "value": 152.8759984791487,
            "unit": "ns/iter",
            "extra": "iterations: 4586976\ncpu: 152.809171009397 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter_with_vibrato",
            "value": 171.2092980553462,
            "unit": "ns/iter",
            "extra": "iterations: 4144415\ncpu: 170.86874745892987 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_full",
            "value": 451.83669099829706,
            "unit": "ns/iter",
            "extra": "iterations: 1559020\ncpu: 451.70235147720956 ns\nthreads: 1"
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
          "id": "04d67d5b0dc5bbc08086b0622fed58359418a383",
          "message": "prefer references to pointers",
          "timestamp": "2021-08-08T00:26:59+09:00",
          "tree_id": "8db55c7803425bf109c3a824aaa30a798b2f2bfb",
          "url": "https://github.com/jinjor/Grape/commit/04d67d5b0dc5bbc08086b0622fed58359418a383"
        },
        "date": 1628351014634,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_VoiceStep_empty",
            "value": 35.902436440620136,
            "unit": "ns/iter",
            "extra": "iterations: 19606470\ncpu: 35.88203281875829 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_whitenoise",
            "value": 76.2184079930483,
            "unit": "ns/iter",
            "extra": "iterations: 9094571\ncpu: 76.09506814560025 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_pinknoise",
            "value": 81.98092071774843,
            "unit": "ns/iter",
            "extra": "iterations: 8568771\ncpu: 81.9690478366151 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine",
            "value": 85.79113933103251,
            "unit": "ns/iter",
            "extra": "iterations: 7896221\ncpu: 85.75089273717137 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_square",
            "value": 91.10139335792087,
            "unit": "ns/iter",
            "extra": "iterations: 7708572\ncpu: 91.07186129934315 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_vibrato",
            "value": 142.60497408474416,
            "unit": "ns/iter",
            "extra": "iterations: 5188452\ncpu: 141.61295122321636 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_tremolo",
            "value": 139.84931202797233,
            "unit": "ns/iter",
            "extra": "iterations: 4856592\ncpu: 139.80832649726412 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison",
            "value": 117.87099961725033,
            "unit": "ns/iter",
            "extra": "iterations: 5590340\ncpu: 117.84721501733334 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison_with_autopan",
            "value": 206.8183326988571,
            "unit": "ns/iter",
            "extra": "iterations: 3293994\ncpu: 205.55319772895757 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_sine",
            "value": 169.01247253574442,
            "unit": "ns/iter",
            "extra": "iterations: 4164510\ncpu: 168.9483276543943 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter",
            "value": 108.82989694221129,
            "unit": "ns/iter",
            "extra": "iterations: 6345465\ncpu: 108.78651131162201 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterfreq",
            "value": 153.14137871015404,
            "unit": "ns/iter",
            "extra": "iterations: 4378092\ncpu: 153.11944107159036 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_lowshelf_with_lfo_filterfreq",
            "value": 163.2743567225572,
            "unit": "ns/iter",
            "extra": "iterations: 4477306\ncpu: 162.54997983162195 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterq",
            "value": 117.79082277689908,
            "unit": "ns/iter",
            "extra": "iterations: 5864955\ncpu: 117.7729411393608 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter",
            "value": 114.90571029362305,
            "unit": "ns/iter",
            "extra": "iterations: 6261797\ncpu: 114.89752861678545 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter_with_vibrato",
            "value": 136.40818440331608,
            "unit": "ns/iter",
            "extra": "iterations: 5020476\ncpu: 136.29106084761682 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_abs_filter",
            "value": 128.43850784028768,
            "unit": "ns/iter",
            "extra": "iterations: 5319109\ncpu: 128.40496406446985 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter",
            "value": 160.0462534966446,
            "unit": "ns/iter",
            "extra": "iterations: 4658437\ncpu: 159.9946505662735 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter_with_vibrato",
            "value": 174.08299454519837,
            "unit": "ns/iter",
            "extra": "iterations: 3837698\ncpu: 173.9941496178184 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_full",
            "value": 456.5648252267164,
            "unit": "ns/iter",
            "extra": "iterations: 1518992\ncpu: 456.49549174715605 ns\nthreads: 1"
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
          "id": "9b5e66b7446673366fb34d7a521832ccb65f00db",
          "message": "add StyleConstants.h",
          "timestamp": "2021-08-08T03:07:25+09:00",
          "tree_id": "bb09a284b3eae76f02b665a263794ad0fb24ab6d",
          "url": "https://github.com/jinjor/Grape/commit/9b5e66b7446673366fb34d7a521832ccb65f00db"
        },
        "date": 1628360792427,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_VoiceStep_empty",
            "value": 36.66720885944404,
            "unit": "ns/iter",
            "extra": "iterations: 19352895\ncpu: 36.458111305827885 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_whitenoise",
            "value": 77.10369671447587,
            "unit": "ns/iter",
            "extra": "iterations: 9283081\ncpu: 76.7203259348917 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_pinknoise",
            "value": 84.56787705672711,
            "unit": "ns/iter",
            "extra": "iterations: 8499271\ncpu: 82.68014986226464 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine",
            "value": 84.27024963130921,
            "unit": "ns/iter",
            "extra": "iterations: 8765888\ncpu: 83.70663645257623 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_square",
            "value": 94.5991849211382,
            "unit": "ns/iter",
            "extra": "iterations: 7503323\ncpu: 94.1404228499826 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_vibrato",
            "value": 144.8708409072652,
            "unit": "ns/iter",
            "extra": "iterations: 4841711\ncpu: 144.13768190625163 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_tremolo",
            "value": 147.41271878576978,
            "unit": "ns/iter",
            "extra": "iterations: 4886630\ncpu: 143.87379441455553 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison",
            "value": 122.49971871622867,
            "unit": "ns/iter",
            "extra": "iterations: 5809080\ncpu: 121.84907076507822 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison_with_autopan",
            "value": 213.47424717848315,
            "unit": "ns/iter",
            "extra": "iterations: 3291484\ncpu: 212.374722161797 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_sine",
            "value": 182.00159296759233,
            "unit": "ns/iter",
            "extra": "iterations: 3951744\ncpu: 179.39395871797362 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter",
            "value": 108.73206960165632,
            "unit": "ns/iter",
            "extra": "iterations: 6679899\ncpu: 108.27469097960918 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterfreq",
            "value": 164.33418079859737,
            "unit": "ns/iter",
            "extra": "iterations: 4294768\ncpu: 162.56035250332508 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_lowshelf_with_lfo_filterfreq",
            "value": 168.29029867180785,
            "unit": "ns/iter",
            "extra": "iterations: 4096202\ncpu: 167.52079121098038 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterq",
            "value": 128.57637174056802,
            "unit": "ns/iter",
            "extra": "iterations: 5608750\ncpu: 127.62790283039922 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter",
            "value": 121.72624141774287,
            "unit": "ns/iter",
            "extra": "iterations: 5799599\ncpu: 118.76528015126566 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter_with_vibrato",
            "value": 142.62040253102913,
            "unit": "ns/iter",
            "extra": "iterations: 5019936\ncpu: 141.77909837894384 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_abs_filter",
            "value": 132.85350977138327,
            "unit": "ns/iter",
            "extra": "iterations: 5404654\ncpu: 132.21530925013877 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter",
            "value": 160.835708732971,
            "unit": "ns/iter",
            "extra": "iterations: 4405785\ncpu: 159.13622657483276 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter_with_vibrato",
            "value": 180.22001646848605,
            "unit": "ns/iter",
            "extra": "iterations: 3874060\ncpu: 177.55687831370744 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_full",
            "value": 474.90503132426625,
            "unit": "ns/iter",
            "extra": "iterations: 1474423\ncpu: 470.13102752737893 ns\nthreads: 1"
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
          "id": "7f1ae1ca56a3b716ce3bf7f4d1aee9a006a7d24e",
          "message": "use normalized angle",
          "timestamp": "2021-08-08T04:15:00+09:00",
          "tree_id": "782553dc88aa2a427817de488a346dac4e442717",
          "url": "https://github.com/jinjor/Grape/commit/7f1ae1ca56a3b716ce3bf7f4d1aee9a006a7d24e"
        },
        "date": 1628364747631,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_VoiceStep_empty",
            "value": 39.994432819366615,
            "unit": "ns/iter",
            "extra": "iterations: 18048633\ncpu: 39.97166987660506 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_whitenoise",
            "value": 85.04468769410407,
            "unit": "ns/iter",
            "extra": "iterations: 9279266\ncpu: 85.01965564948782 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_pinknoise",
            "value": 84.00976630057927,
            "unit": "ns/iter",
            "extra": "iterations: 8548887\ncpu: 83.99970662847689 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine",
            "value": 82.84287246483369,
            "unit": "ns/iter",
            "extra": "iterations: 8269149\ncpu: 82.73705069288262 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_square",
            "value": 101.05370862099008,
            "unit": "ns/iter",
            "extra": "iterations: 7727549\ncpu: 101.00951802440855 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_vibrato",
            "value": 148.33994576379493,
            "unit": "ns/iter",
            "extra": "iterations: 5068938\ncpu: 148.2687300574598 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_tremolo",
            "value": 140.05160239539478,
            "unit": "ns/iter",
            "extra": "iterations: 4208855\ncpu: 140.00482316449472 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison",
            "value": 115.5043012672598,
            "unit": "ns/iter",
            "extra": "iterations: 5983469\ncpu: 115.45693643603744 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison_with_autopan",
            "value": 237.11994757445058,
            "unit": "ns/iter",
            "extra": "iterations: 2811595\ncpu: 237.0629482553497 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_sine",
            "value": 189.17983098665994,
            "unit": "ns/iter",
            "extra": "iterations: 3776863\ncpu: 189.1397702273027 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter",
            "value": 106.59190092648595,
            "unit": "ns/iter",
            "extra": "iterations: 6357221\ncpu: 106.5256343927639 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterfreq",
            "value": 163.49014291364705,
            "unit": "ns/iter",
            "extra": "iterations: 4368938\ncpu: 163.38341262796607 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_lowshelf_with_lfo_filterfreq",
            "value": 163.39287141654424,
            "unit": "ns/iter",
            "extra": "iterations: 4318642\ncpu: 163.29832387125373 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterq",
            "value": 134.3295751656572,
            "unit": "ns/iter",
            "extra": "iterations: 5921414\ncpu: 134.28042693856574 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter",
            "value": 129.31256041099854,
            "unit": "ns/iter",
            "extra": "iterations: 5585701\ncpu: 129.26542254947043 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter_with_vibrato",
            "value": 143.58747380555093,
            "unit": "ns/iter",
            "extra": "iterations: 4866011\ncpu: 143.5249529851043 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_abs_filter",
            "value": 149.51297130583612,
            "unit": "ns/iter",
            "extra": "iterations: 4431782\ncpu: 149.4466559952632 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter",
            "value": 186.68118012944507,
            "unit": "ns/iter",
            "extra": "iterations: 4035862\ncpu: 186.59334734438377 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter_with_vibrato",
            "value": 209.72632235663627,
            "unit": "ns/iter",
            "extra": "iterations: 3415701\ncpu: 209.6781890452351 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_full",
            "value": 509.1044399996463,
            "unit": "ns/iter",
            "extra": "iterations: 1000000\ncpu: 508.8700000000017 ns\nthreads: 1"
          },
          {
            "name": "BM_DelayStep",
            "value": 34.58754715183686,
            "unit": "ns/iter",
            "extra": "iterations: 16424869\ncpu: 34.57707942754355 ns\nthreads: 1"
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
          "id": "ffe95654fd05ce94666a7e8dc1adfb86b033440e",
          "message": "cache more",
          "timestamp": "2021-08-08T05:10:13+09:00",
          "tree_id": "c076c8fa6413a410baf686943265e15013485466",
          "url": "https://github.com/jinjor/Grape/commit/ffe95654fd05ce94666a7e8dc1adfb86b033440e"
        },
        "date": 1628384190607,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_VoiceStep_empty",
            "value": 37.73339874690385,
            "unit": "ns/iter",
            "extra": "iterations: 19051092\ncpu: 36.65595651944781 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_whitenoise",
            "value": 75.98264073931604,
            "unit": "ns/iter",
            "extra": "iterations: 9551098\ncpu: 75.5062925749479 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_pinknoise",
            "value": 87.36500824117245,
            "unit": "ns/iter",
            "extra": "iterations: 8579483\ncpu: 80.74530831286694 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine",
            "value": 83.80742545565943,
            "unit": "ns/iter",
            "extra": "iterations: 8785912\ncpu: 79.29751629654383 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_square",
            "value": 96.28539254334379,
            "unit": "ns/iter",
            "extra": "iterations: 7847006\ncpu: 88.98999185166933 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_vibrato",
            "value": 139.7403738182977,
            "unit": "ns/iter",
            "extra": "iterations: 5251589\ncpu: 136.18030657006852 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_tremolo",
            "value": 140.1276221837286,
            "unit": "ns/iter",
            "extra": "iterations: 5237953\ncpu: 138.12285066322673 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison",
            "value": 130.5373018492096,
            "unit": "ns/iter",
            "extra": "iterations: 6175726\ncpu: 119.83789436254146 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison_with_autopan",
            "value": 207.31369882518405,
            "unit": "ns/iter",
            "extra": "iterations: 3245052\ncpu: 202.08058299219863 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_sine",
            "value": 168.43897858685418,
            "unit": "ns/iter",
            "extra": "iterations: 4313240\ncpu: 161.9179549480205 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter",
            "value": 105.00358196777871,
            "unit": "ns/iter",
            "extra": "iterations: 6676777\ncpu: 102.28572857832444 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterfreq",
            "value": 160.17804593811894,
            "unit": "ns/iter",
            "extra": "iterations: 4512723\ncpu: 155.34102137445603 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_lowshelf_with_lfo_filterfreq",
            "value": 169.34429859974722,
            "unit": "ns/iter",
            "extra": "iterations: 4427465\ncpu: 159.07251666585734 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterq",
            "value": 119.12861922426802,
            "unit": "ns/iter",
            "extra": "iterations: 5912662\ncpu: 116.93447046355782 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter",
            "value": 125.94038357944004,
            "unit": "ns/iter",
            "extra": "iterations: 5923519\ncpu: 120.78327089015845 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter_with_vibrato",
            "value": 150.03208323416118,
            "unit": "ns/iter",
            "extra": "iterations: 5117034\ncpu: 138.70886923948487 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_abs_filter",
            "value": 130.6083583979084,
            "unit": "ns/iter",
            "extra": "iterations: 5507778\ncpu: 128.1173642074897 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter",
            "value": 160.07930018319155,
            "unit": "ns/iter",
            "extra": "iterations: 4557707\ncpu: 154.28262501297252 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter_with_vibrato",
            "value": 181.44516870544052,
            "unit": "ns/iter",
            "extra": "iterations: 4029241\ncpu: 174.28766360711606 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_full",
            "value": 487.4920428159028,
            "unit": "ns/iter",
            "extra": "iterations: 1490100\ncpu: 465.7700825447944 ns\nthreads: 1"
          },
          {
            "name": "BM_DelayStep",
            "value": 35.553100373231715,
            "unit": "ns/iter",
            "extra": "iterations: 21060803\ncpu: 32.972959293147426 ns\nthreads: 1"
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
          "id": "7c52ad138de038bb653692dc8a9d8054b12afc98",
          "message": "cache more",
          "timestamp": "2021-08-08T17:33:10+09:00",
          "tree_id": "0c864dbe1bdbd841bc54b82ef15d20e5092e114c",
          "url": "https://github.com/jinjor/Grape/commit/7c52ad138de038bb653692dc8a9d8054b12afc98"
        },
        "date": 1628412862896,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_VoiceStep_empty",
            "value": 37.02450197119638,
            "unit": "ns/iter",
            "extra": "iterations: 19623564\ncpu: 36.512429648355415 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_whitenoise",
            "value": 74.1171344586138,
            "unit": "ns/iter",
            "extra": "iterations: 10167768\ncpu: 69.82250185094705 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_pinknoise",
            "value": 85.73567513758358,
            "unit": "ns/iter",
            "extra": "iterations: 9220839\ncpu: 77.62569111118843 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine",
            "value": 87.12738981170733,
            "unit": "ns/iter",
            "extra": "iterations: 9234828\ncpu: 76.47549039354061 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_square",
            "value": 88.4961782423312,
            "unit": "ns/iter",
            "extra": "iterations: 8344459\ncpu: 84.46371418446657 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_vibrato",
            "value": 139.405168006271,
            "unit": "ns/iter",
            "extra": "iterations: 5101000\ncpu: 137.29896098804142 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_tremolo",
            "value": 177.85655990195818,
            "unit": "ns/iter",
            "extra": "iterations: 5226572\ncpu: 134.7363051728744 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison",
            "value": 170.06348901352322,
            "unit": "ns/iter",
            "extra": "iterations: 6026586\ncpu: 116.11316921387987 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison_with_autopan",
            "value": 235.65513590720025,
            "unit": "ns/iter",
            "extra": "iterations: 3459934\ncpu: 201.78853122631818 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_sine",
            "value": 161.25496328937248,
            "unit": "ns/iter",
            "extra": "iterations: 4876655\ncpu: 144.0719099464694 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter",
            "value": 158.8554896962629,
            "unit": "ns/iter",
            "extra": "iterations: 6944100\ncpu: 103.83116602583492 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterfreq",
            "value": 186.71651473759164,
            "unit": "ns/iter",
            "extra": "iterations: 4646686\ncpu: 149.6036099706326 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_lowshelf_with_lfo_filterfreq",
            "value": 156.6465088971331,
            "unit": "ns/iter",
            "extra": "iterations: 4605536\ncpu: 153.05449789123313 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterq",
            "value": 118.86615714327677,
            "unit": "ns/iter",
            "extra": "iterations: 5911014\ncpu: 116.8102460931407 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter",
            "value": 139.11164423714882,
            "unit": "ns/iter",
            "extra": "iterations: 6502736\ncpu: 106.45918887065378 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter_with_vibrato",
            "value": 173.34763242249068,
            "unit": "ns/iter",
            "extra": "iterations: 5404237\ncpu: 129.98005083788897 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_abs_filter",
            "value": 170.6576605426311,
            "unit": "ns/iter",
            "extra": "iterations: 5589715\ncpu: 125.71678520282346 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter",
            "value": 221.95538932457671,
            "unit": "ns/iter",
            "extra": "iterations: 4751755\ncpu: 148.35234560704407 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter_with_vibrato",
            "value": 292.30728047092197,
            "unit": "ns/iter",
            "extra": "iterations: 4206857\ncpu: 168.70670907045346 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_full",
            "value": 540.2408075275766,
            "unit": "ns/iter",
            "extra": "iterations: 1598509\ncpu: 440.4585773367567 ns\nthreads: 1"
          },
          {
            "name": "BM_DelayStep",
            "value": 43.874000309392294,
            "unit": "ns/iter",
            "extra": "iterations: 20964361\ncpu: 33.23111064534712 ns\nthreads: 1"
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
          "id": "eb8ab082de3aef7144fdff15be922a4557b297b0",
          "message": "update ci",
          "timestamp": "2021-08-08T18:11:01+09:00",
          "tree_id": "3dc959e9075433b77a9f1eef8b00adeee34f9b91",
          "url": "https://github.com/jinjor/Grape/commit/eb8ab082de3aef7144fdff15be922a4557b297b0"
        },
        "date": 1628414855925,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_VoiceStep_empty",
            "value": 36.065411892087454,
            "unit": "ns/iter",
            "extra": "iterations: 19755261\ncpu: 36.05257353977758 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_whitenoise",
            "value": 67.48744019799412,
            "unit": "ns/iter",
            "extra": "iterations: 10251754\ncpu: 67.4749901333957 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_pinknoise",
            "value": 74.01810086041559,
            "unit": "ns/iter",
            "extra": "iterations: 9039957\ncpu: 73.9705952141144 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine",
            "value": 73.53755554773956,
            "unit": "ns/iter",
            "extra": "iterations: 9587465\ncpu: 73.50639611200663 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_square",
            "value": 81.51633410893089,
            "unit": "ns/iter",
            "extra": "iterations: 8350930\ncpu: 81.4970308696157 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_vibrato",
            "value": 133.05151641173174,
            "unit": "ns/iter",
            "extra": "iterations: 4910144\ncpu: 133.02033504516365 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_tremolo",
            "value": 129.77176161348297,
            "unit": "ns/iter",
            "extra": "iterations: 5346042\ncpu: 129.73897324413096 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison",
            "value": 116.81877259626218,
            "unit": "ns/iter",
            "extra": "iterations: 6106124\ncpu: 116.77522434853927 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison_with_autopan",
            "value": 203.10149613548998,
            "unit": "ns/iter",
            "extra": "iterations: 3444407\ncpu: 203.07443342206656 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_sine",
            "value": 143.85960739487382,
            "unit": "ns/iter",
            "extra": "iterations: 4795046\ncpu: 143.82531471022386 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter",
            "value": 102.97569348096079,
            "unit": "ns/iter",
            "extra": "iterations: 7030583\ncpu: 102.94893040875846 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterfreq",
            "value": 146.50512431506027,
            "unit": "ns/iter",
            "extra": "iterations: 4812253\ncpu: 146.45967283931242 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_lowshelf_with_lfo_filterfreq",
            "value": 148.11977203655687,
            "unit": "ns/iter",
            "extra": "iterations: 4711108\ncpu: 148.0883053413339 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterq",
            "value": 111.5072150963401,
            "unit": "ns/iter",
            "extra": "iterations: 6118560\ncpu: 111.49224654166986 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter",
            "value": 103.41124931365815,
            "unit": "ns/iter",
            "extra": "iterations: 6811260\ncpu: 103.38527673293949 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter_with_vibrato",
            "value": 129.87153303141974,
            "unit": "ns/iter",
            "extra": "iterations: 5483573\ncpu: 129.83760770577862 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_abs_filter",
            "value": 123.06695071214014,
            "unit": "ns/iter",
            "extra": "iterations: 5840147\ncpu: 123.04073852935531 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter",
            "value": 149.3671795667239,
            "unit": "ns/iter",
            "extra": "iterations: 4856828\ncpu: 149.31597330603418 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter_with_vibrato",
            "value": 159.41081912993087,
            "unit": "ns/iter",
            "extra": "iterations: 4031840\ncpu: 159.383308861463 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_full",
            "value": 423.452147091908,
            "unit": "ns/iter",
            "extra": "iterations: 1657684\ncpu: 423.3840707879193 ns\nthreads: 1"
          },
          {
            "name": "BM_DelayStep",
            "value": 32.19402927667027,
            "unit": "ns/iter",
            "extra": "iterations: 22176040\ncpu: 32.1786035739474 ns\nthreads: 1"
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
          "id": "aac536f424dd6f7bab2edb9ad7627531849e7eac",
          "message": "clang-format everywhere",
          "timestamp": "2021-08-08T23:55:42+09:00",
          "tree_id": "f51581c12894b1eb3ec3bf485245d6b6493c5bc1",
          "url": "https://github.com/jinjor/Grape/commit/aac536f424dd6f7bab2edb9ad7627531849e7eac"
        },
        "date": 1628435901842,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_VoiceStep_empty",
            "value": 41.837025429762754,
            "unit": "ns/iter",
            "extra": "iterations: 18432888\ncpu: 38.42506936514777 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_whitenoise",
            "value": 77.31149345417593,
            "unit": "ns/iter",
            "extra": "iterations: 9738317\ncpu: 75.64993006491778 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_pinknoise",
            "value": 88.12495419449807,
            "unit": "ns/iter",
            "extra": "iterations: 8522451\ncpu: 81.9374614180827 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine",
            "value": 86.17908073103068,
            "unit": "ns/iter",
            "extra": "iterations: 8822010\ncpu: 82.70144785598745 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_square",
            "value": 98.78729693124617,
            "unit": "ns/iter",
            "extra": "iterations: 7708657\ncpu: 95.36953064586993 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_vibrato",
            "value": 157.93770141440422,
            "unit": "ns/iter",
            "extra": "iterations: 4404787\ncpu: 155.36755806807457 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_tremolo",
            "value": 159.76571991353396,
            "unit": "ns/iter",
            "extra": "iterations: 4870717\ncpu: 146.20845349873537 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison",
            "value": 127.45946952410503,
            "unit": "ns/iter",
            "extra": "iterations: 5354588\ncpu: 123.6356560019183 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison_with_autopan",
            "value": 236.2843448673937,
            "unit": "ns/iter",
            "extra": "iterations: 3255814\ncpu: 213.89919694429696 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_sine",
            "value": 155.24509099046278,
            "unit": "ns/iter",
            "extra": "iterations: 4754014\ncpu: 153.43328816448619 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter",
            "value": 111.96666188224113,
            "unit": "ns/iter",
            "extra": "iterations: 6381854\ncpu: 106.21615599479402 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterfreq",
            "value": 155.92474661612118,
            "unit": "ns/iter",
            "extra": "iterations: 4373305\ncpu: 154.78819794183107 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_lowshelf_with_lfo_filterfreq",
            "value": 168.1565739867389,
            "unit": "ns/iter",
            "extra": "iterations: 4207059\ncpu: 164.17882420950104 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterq",
            "value": 124.85781359231775,
            "unit": "ns/iter",
            "extra": "iterations: 5773196\ncpu: 120.60408134419815 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter",
            "value": 115.69380648072088,
            "unit": "ns/iter",
            "extra": "iterations: 6431400\ncpu: 113.11254159281037 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter_with_vibrato",
            "value": 139.12090694643237,
            "unit": "ns/iter",
            "extra": "iterations: 5213803\ncpu: 133.94464654686803 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_abs_filter",
            "value": 133.94777139689953,
            "unit": "ns/iter",
            "extra": "iterations: 5327732\ncpu: 132.09560841273557 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter",
            "value": 159.22675634388435,
            "unit": "ns/iter",
            "extra": "iterations: 4538647\ncpu: 155.4412581546877 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter_with_vibrato",
            "value": 179.0477128403718,
            "unit": "ns/iter",
            "extra": "iterations: 3820481\ncpu: 175.1054382942884 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_full",
            "value": 473.11404539506236,
            "unit": "ns/iter",
            "extra": "iterations: 1495273\ncpu: 461.71568670069064 ns\nthreads: 1"
          },
          {
            "name": "BM_DelayStep",
            "value": 37.56210665613661,
            "unit": "ns/iter",
            "extra": "iterations: 20480172\ncpu: 35.688372148436805 ns\nthreads: 1"
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
          "distinct": false,
          "id": "4b2e065a7d43b143a41a8398bcc908707da2f82a",
          "message": "conditionally call forceStop()",
          "timestamp": "2021-08-14T05:15:42+09:00",
          "tree_id": "11255ab8cd4481eec695e84851ba859092957906",
          "url": "https://github.com/jinjor/Grape/commit/4b2e065a7d43b143a41a8398bcc908707da2f82a"
        },
        "date": 1629508597705,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_VoiceStep_empty",
            "value": 35.54552004861773,
            "unit": "ns/iter",
            "extra": "iterations: 19742279\ncpu: 35.49108995977618 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_whitenoise",
            "value": 67.61879645891412,
            "unit": "ns/iter",
            "extra": "iterations: 10320678\ncpu: 67.56949494984727 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_pinknoise",
            "value": 74.14534797491524,
            "unit": "ns/iter",
            "extra": "iterations: 9493328\ncpu: 74.09245735531314 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine",
            "value": 73.616425966398,
            "unit": "ns/iter",
            "extra": "iterations: 9512938\ncpu: 73.57821526851112 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_square",
            "value": 82.92343681745909,
            "unit": "ns/iter",
            "extra": "iterations: 8426831\ncpu: 82.80835346051211 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_vibrato",
            "value": 138.75330817162788,
            "unit": "ns/iter",
            "extra": "iterations: 5069341\ncpu: 135.14300971270234 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_tremolo",
            "value": 135.34918821396934,
            "unit": "ns/iter",
            "extra": "iterations: 5344327\ncpu: 131.3862344126772 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison",
            "value": 113.41040848762024,
            "unit": "ns/iter",
            "extra": "iterations: 6210739\ncpu: 112.91957366104099 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison_with_autopan",
            "value": 201.16851499183656,
            "unit": "ns/iter",
            "extra": "iterations: 3520755\ncpu: 200.76716499728056 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_sine",
            "value": 141.91816591932377,
            "unit": "ns/iter",
            "extra": "iterations: 4989486\ncpu: 141.62981918377955 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter",
            "value": 98.63573042302686,
            "unit": "ns/iter",
            "extra": "iterations: 7118813\ncpu: 98.24123768948554 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterfreq",
            "value": 144.89402760190816,
            "unit": "ns/iter",
            "extra": "iterations: 4849593\ncpu: 144.70760742190066 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_lowshelf_with_lfo_filterfreq",
            "value": 148.77421892121015,
            "unit": "ns/iter",
            "extra": "iterations: 4714217\ncpu: 148.6138631293386 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterq",
            "value": 114.27096554546327,
            "unit": "ns/iter",
            "extra": "iterations: 6165282\ncpu: 114.19802046362189 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter",
            "value": 104.36303027766031,
            "unit": "ns/iter",
            "extra": "iterations: 6761717\ncpu: 104.28401543572424 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter_with_vibrato",
            "value": 125.54043096763576,
            "unit": "ns/iter",
            "extra": "iterations: 5566822\ncpu: 125.4550621521581 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_abs_filter",
            "value": 122.8845894414989,
            "unit": "ns/iter",
            "extra": "iterations: 5732959\ncpu: 122.81423955761758 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter",
            "value": 142.72529659030224,
            "unit": "ns/iter",
            "extra": "iterations: 4901206\ncpu: 142.563687386329 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter_with_vibrato",
            "value": 160.27811522120595,
            "unit": "ns/iter",
            "extra": "iterations: 4375219\ncpu: 160.14695492957034 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_full",
            "value": 454.25247602709965,
            "unit": "ns/iter",
            "extra": "iterations: 1631949\ncpu: 432.7384005259963 ns\nthreads: 1"
          },
          {
            "name": "BM_DelayStep",
            "value": 32.138396737978205,
            "unit": "ns/iter",
            "extra": "iterations: 21886354\ncpu: 32.11946585529969 ns\nthreads: 1"
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
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "21a732e11edca4c64b79de7bd611bdf471f4d88a",
          "message": "Merge pull request #13 from jinjor/expression\n\nfix expression",
          "timestamp": "2021-08-21T10:02:36+09:00",
          "tree_id": "c5e16627bbac60d27c4fe29e972b45d61349fba8",
          "url": "https://github.com/jinjor/Grape/commit/21a732e11edca4c64b79de7bd611bdf471f4d88a"
        },
        "date": 1629508767737,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_VoiceStep_empty",
            "value": 35.97840883272839,
            "unit": "ns/iter",
            "extra": "iterations: 18850903\ncpu: 35.84491416671126 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_whitenoise",
            "value": 68.2699468555821,
            "unit": "ns/iter",
            "extra": "iterations: 10167768\ncpu: 68.2253961734768 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_pinknoise",
            "value": 74.68324640718251,
            "unit": "ns/iter",
            "extra": "iterations: 9401273\ncpu: 74.63723263860119 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine",
            "value": 74.81048205377533,
            "unit": "ns/iter",
            "extra": "iterations: 9421646\ncpu: 74.51903839307903 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_square",
            "value": 82.4504701815885,
            "unit": "ns/iter",
            "extra": "iterations: 8471089\ncpu: 82.3672139438034 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_vibrato",
            "value": 131.69521011425763,
            "unit": "ns/iter",
            "extra": "iterations: 5400609\ncpu: 131.63626546561696 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_tremolo",
            "value": 129.67341220112706,
            "unit": "ns/iter",
            "extra": "iterations: 5431198\ncpu: 129.61063102468376 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison",
            "value": 110.90177703258539,
            "unit": "ns/iter",
            "extra": "iterations: 6215868\ncpu: 110.88925955313066 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison_with_autopan",
            "value": 200.6176754659606,
            "unit": "ns/iter",
            "extra": "iterations: 3448803\ncpu: 200.34922261433903 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_sine",
            "value": 149.3409336999174,
            "unit": "ns/iter",
            "extra": "iterations: 4630610\ncpu: 149.20431649393947 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter",
            "value": 98.82099433729203,
            "unit": "ns/iter",
            "extra": "iterations: 7158416\ncpu: 98.77883598829666 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterfreq",
            "value": 147.70019136420817,
            "unit": "ns/iter",
            "extra": "iterations: 4458514\ncpu: 147.6251953004966 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_lowshelf_with_lfo_filterfreq",
            "value": 153.71397592680088,
            "unit": "ns/iter",
            "extra": "iterations: 4613762\ncpu: 153.64013141553474 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterq",
            "value": 114.5845167540388,
            "unit": "ns/iter",
            "extra": "iterations: 5997515\ncpu: 114.54360681048745 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter",
            "value": 103.48218936543756,
            "unit": "ns/iter",
            "extra": "iterations: 6488090\ncpu: 103.45247985154353 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter_with_vibrato",
            "value": 125.07160255906894,
            "unit": "ns/iter",
            "extra": "iterations: 5744250\ncpu: 125.01632066849437 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_abs_filter",
            "value": 134.7393404466903,
            "unit": "ns/iter",
            "extra": "iterations: 5699676\ncpu: 128.34834822189916 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter",
            "value": 143.6176113408346,
            "unit": "ns/iter",
            "extra": "iterations: 4575881\ncpu: 143.54328707411779 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter_with_vibrato",
            "value": 164.4177456573632,
            "unit": "ns/iter",
            "extra": "iterations: 4313213\ncpu: 164.3855752080878 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_full",
            "value": 420.07471053202676,
            "unit": "ns/iter",
            "extra": "iterations: 1649232\ncpu: 420.0064029802965 ns\nthreads: 1"
          },
          {
            "name": "BM_DelayStep",
            "value": 31.907631182926384,
            "unit": "ns/iter",
            "extra": "iterations: 21276531\ncpu: 31.903133081234007 ns\nthreads: 1"
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
          "id": "cf295c9cab0feee0bc050d4ca6e618bb87ca66e2",
          "message": "attempt to make analyser work less",
          "timestamp": "2021-08-21T14:05:59+09:00",
          "tree_id": "915050783ff43b165155846bbc0bfa85c89f0dcd",
          "url": "https://github.com/jinjor/Grape/commit/cf295c9cab0feee0bc050d4ca6e618bb87ca66e2"
        },
        "date": 1629530027510,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_VoiceStep_empty",
            "value": 36.701989616351995,
            "unit": "ns/iter",
            "extra": "iterations: 19013163\ncpu: 36.661759014005185 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_whitenoise",
            "value": 70.55631926418096,
            "unit": "ns/iter",
            "extra": "iterations: 10093873\ncpu: 70.42579196310474 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_pinknoise",
            "value": 76.6801426716595,
            "unit": "ns/iter",
            "extra": "iterations: 9209678\ncpu: 76.58856259686821 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine",
            "value": 75.97903559237847,
            "unit": "ns/iter",
            "extra": "iterations: 9047668\ncpu: 75.88452626687898 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_square",
            "value": 84.83215200430948,
            "unit": "ns/iter",
            "extra": "iterations: 8263292\ncpu: 84.7832800777221 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_vibrato",
            "value": 134.09014382312503,
            "unit": "ns/iter",
            "extra": "iterations: 5162661\ncpu: 134.0006636112657 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_tremolo",
            "value": 153.6655950884789,
            "unit": "ns/iter",
            "extra": "iterations: 5212250\ncpu: 138.34831406782095 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison",
            "value": 118.7498813019911,
            "unit": "ns/iter",
            "extra": "iterations: 6107936\ncpu: 116.60878568472233 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison_with_autopan",
            "value": 204.60050143291207,
            "unit": "ns/iter",
            "extra": "iterations: 3479229\ncpu: 201.55040096527142 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_sine",
            "value": 152.97002831819685,
            "unit": "ns/iter",
            "extra": "iterations: 4572683\ncpu: 152.24059922806825 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter",
            "value": 99.94249748255554,
            "unit": "ns/iter",
            "extra": "iterations: 6998111\ncpu: 99.86037660734418 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterfreq",
            "value": 151.05085724562704,
            "unit": "ns/iter",
            "extra": "iterations: 4559370\ncpu: 150.98226290035706 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_lowshelf_with_lfo_filterfreq",
            "value": 156.36531850521666,
            "unit": "ns/iter",
            "extra": "iterations: 4496865\ncpu: 156.25396804218045 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterq",
            "value": 116.34510559542767,
            "unit": "ns/iter",
            "extra": "iterations: 6062811\ncpu: 116.24129467337835 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter",
            "value": 106.10160010972878,
            "unit": "ns/iter",
            "extra": "iterations: 6597985\ncpu: 106.02115645913064 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter_with_vibrato",
            "value": 127.35346374629918,
            "unit": "ns/iter",
            "extra": "iterations: 5540605\ncpu: 127.25000248167822 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_abs_filter",
            "value": 125.99975531625769,
            "unit": "ns/iter",
            "extra": "iterations: 5582716\ncpu: 125.86060261707763 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter",
            "value": 145.09458253714024,
            "unit": "ns/iter",
            "extra": "iterations: 4792354\ncpu: 144.9154215235351 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter_with_vibrato",
            "value": 166.2124113280879,
            "unit": "ns/iter",
            "extra": "iterations: 4208349\ncpu: 165.9482138957591 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_full",
            "value": 434.506135245874,
            "unit": "ns/iter",
            "extra": "iterations: 1618680\ncpu: 434.3032594459667 ns\nthreads: 1"
          },
          {
            "name": "BM_DelayStep",
            "value": 35.702355366828996,
            "unit": "ns/iter",
            "extra": "iterations: 21215464\ncpu: 33.35779033633203 ns\nthreads: 1"
          }
        ]
      }
    ]
  }
}