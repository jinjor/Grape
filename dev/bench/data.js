window.BENCHMARK_DATA = {
  "lastUpdate": 1627662735511,
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
          "id": "7cf4a5b9019fd2df442670f6c352e6500ac8f0bb",
          "message": "try leoholz/github-action-benchmark",
          "timestamp": "2021-07-31T00:34:13+09:00",
          "tree_id": "f56caf36bf885e70fb1321342ad5b5640ad18f0e",
          "url": "https://github.com/jinjor/Grape/commit/7cf4a5b9019fd2df442670f6c352e6500ac8f0bb"
        },
        "date": 1627660391044,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_VoiceStep_empty",
            "value": 59.51098086538299,
            "unit": "ns/iter",
            "extra": "iterations: 9166263\ncpu: 59.45749101896814 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_whitenoise",
            "value": 60.415094508318035,
            "unit": "ns/iter",
            "extra": "iterations: 12974255\ncpu: 60.373485799377306 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_pinknoise",
            "value": 55.08827385917893,
            "unit": "ns/iter",
            "extra": "iterations: 12626490\ncpu: 55.06645156334026 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine",
            "value": 56.11045204196491,
            "unit": "ns/iter",
            "extra": "iterations: 12187054\ncpu: 56.094360458237055 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_square",
            "value": 60.20741880753347,
            "unit": "ns/iter",
            "extra": "iterations: 13074581\ncpu: 60.16215739533069 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_vibrato",
            "value": 72.7403875964012,
            "unit": "ns/iter",
            "extra": "iterations: 9695962\ncpu: 72.69706708834048 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_tremolo",
            "value": 74.34659759813553,
            "unit": "ns/iter",
            "extra": "iterations: 7576824\ncpu: 74.30488025061686 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison",
            "value": 57.54043342296399,
            "unit": "ns/iter",
            "extra": "iterations: 11427452\ncpu: 57.510458149375665 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison_with_autopan",
            "value": 74.7792114624156,
            "unit": "ns/iter",
            "extra": "iterations: 10189673\ncpu: 74.74371356176 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_sine",
            "value": 69.27700115722939,
            "unit": "ns/iter",
            "extra": "iterations: 11403252\ncpu: 69.2222709802432 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter",
            "value": 119.9535208245534,
            "unit": "ns/iter",
            "extra": "iterations: 5904881\ncpu: 119.86236471149893 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterfreq",
            "value": 113.86587778462177,
            "unit": "ns/iter",
            "extra": "iterations: 6168814\ncpu: 113.82090625523833 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterq",
            "value": 145.66579650554453,
            "unit": "ns/iter",
            "extra": "iterations: 6652791\ncpu: 145.599192880101 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter",
            "value": 134.18859729264273,
            "unit": "ns/iter",
            "extra": "iterations: 5133658\ncpu: 134.10729736963359 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_abs_filter",
            "value": 140.83857168311633,
            "unit": "ns/iter",
            "extra": "iterations: 5202179\ncpu: 140.75947790339384 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter",
            "value": 150.80683574781173,
            "unit": "ns/iter",
            "extra": "iterations: 4141522\ncpu: 150.7704172524009 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_full",
            "value": 234.79862096763276,
            "unit": "ns/iter",
            "extra": "iterations: 2808346\ncpu: 234.74564743802958 ns\nthreads: 1"
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
          "id": "d115d519696ea72e03ee46aaa1ead17520b6b39d",
          "message": "Merge pull request #9 from jinjor/improve-performance\n\nImprove performance",
          "timestamp": "2021-07-31T01:13:34+09:00",
          "tree_id": "4a600150fe0c4f75aec0fe58019d80d378b1ac3d",
          "url": "https://github.com/jinjor/Grape/commit/d115d519696ea72e03ee46aaa1ead17520b6b39d"
        },
        "date": 1627662733527,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_VoiceStep_empty",
            "value": 51.5462572962236,
            "unit": "ns/iter",
            "extra": "iterations: 13725759\ncpu: 51.4987185772386 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_whitenoise",
            "value": 52.377261888435044,
            "unit": "ns/iter",
            "extra": "iterations: 13511166\ncpu: 52.35898959423636 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_pinknoise",
            "value": 52.15963854343518,
            "unit": "ns/iter",
            "extra": "iterations: 13402772\ncpu: 52.14025874647423 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine",
            "value": 52.08391598189767,
            "unit": "ns/iter",
            "extra": "iterations: 13502565\ncpu: 52.06766269964264 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_square",
            "value": 52.241439035278574,
            "unit": "ns/iter",
            "extra": "iterations: 13282732\ncpu: 52.22728276080554 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_vibrato",
            "value": 60.17243783235253,
            "unit": "ns/iter",
            "extra": "iterations: 11735122\ncpu: 60.14219536874012 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_with_tremolo",
            "value": 61.0453949935645,
            "unit": "ns/iter",
            "extra": "iterations: 11357552\ncpu: 61.01649369512024 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison",
            "value": 52.342483440519864,
            "unit": "ns/iter",
            "extra": "iterations: 13297367\ncpu: 52.28403487697979 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_sine_unison_with_autopan",
            "value": 61.16318251979557,
            "unit": "ns/iter",
            "extra": "iterations: 11251487\ncpu: 61.13289736725465 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_sine",
            "value": 57.201424980962045,
            "unit": "ns/iter",
            "extra": "iterations: 12397499\ncpu: 57.17169245184048 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter",
            "value": 79.91732005549584,
            "unit": "ns/iter",
            "extra": "iterations: 8824123\ncpu: 79.88793900538339 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterfreq",
            "value": 91.41399025967668,
            "unit": "ns/iter",
            "extra": "iterations: 7626020\ncpu: 91.37151489243415 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_abs_filter_with_lfo_filterq",
            "value": 88.13101067301098,
            "unit": "ns/iter",
            "extra": "iterations: 8025682\ncpu: 88.08534900834587 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_single_rel_filter",
            "value": 90.81434022355091,
            "unit": "ns/iter",
            "extra": "iterations: 7757006\ncpu: 90.76478734192028 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_abs_filter",
            "value": 103.25972736415119,
            "unit": "ns/iter",
            "extra": "iterations: 6890510\ncpu: 103.20629387374798 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_multiple_rel_filter",
            "value": 130.2399331879618,
            "unit": "ns/iter",
            "extra": "iterations: 5482843\ncpu: 130.19194603967327 ns\nthreads: 1"
          },
          {
            "name": "BM_VoiceStep_full",
            "value": 189.26810125488677,
            "unit": "ns/iter",
            "extra": "iterations: 3636723\ncpu: 189.21952538040418 ns\nthreads: 1"
          }
        ]
      }
    ]
  }
}