window.BENCHMARK_DATA = {
  "lastUpdate": 1628364748741,
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
      }
    ]
  }
}