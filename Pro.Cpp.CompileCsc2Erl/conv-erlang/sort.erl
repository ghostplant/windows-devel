-module(sort).
-export([entry_main/0, generate/1, sort/2, output/1, main/1]).

generate(_arr) ->
  generate_b1(_arr).

generate_b1(_arr) ->
  Id1_i = 0,
  generate_b2(Id1_i, _arr).

generate_b2(Id2_i, Id1_arr) ->
  case (Id2_i < 100) of
    false -> generate_b4(Id1_arr);
    true -> generate_b3(Id2_i, Id1_arr)
  end.

generate_b3(Id2_i, Id1_arr) ->
  T2 = 89 * Id2_i,
  T3 = T2 * Id2_i,
  T4 = T3 rem 101,
  Id2_arr = setelement(1 + Id2_i, Id1_arr, T4),
  T6 = Id2_i + 1,
  Id3_i = T6,
  generate_b2(Id3_i, Id2_arr).

generate_b4(_arr) ->
  {_arr}.


sort(_times, _arr) ->
  sort_b1(_arr, _times).

sort_b1(_arr, _times) ->
  Id1_i = 1,
  sort_b2(Id1_i, _arr, _times).

sort_b2(Id2_i, Id1_arr, _times) ->
  case (Id2_i < 100) of
    false -> sort_b6(Id1_arr, _times);
    true -> sort_b3(Id2_i, Id1_arr, _times)
  end.

sort_b3(Id2_i, Id1_arr, _times) ->
  T2 = Id2_i - 1,
  T3 = element(1 + T2, Id1_arr),
  T4 = element(1 + Id2_i, Id1_arr),
  case (T3 > T4) of
    false -> sort_b5(Id2_i, Id1_arr, _times);
    true -> sort_b4(Id2_i, Id1_arr, _times)
  end.

sort_b6(_arr, Id0_times) ->
  case (Id0_times < 100) of
    false -> sort_b8(_arr);
    true -> sort_b7(_arr, Id0_times)
  end.

sort_b4(Id2_i, Id1_arr, _times) ->
  T6 = Id2_i - 1,
  T7 = element(1 + T6, Id1_arr),
  Id2_tmp = T7,
  T9 = Id2_i - 1,
  T10 = element(1 + Id2_i, Id1_arr),
  Id2_arr = setelement(1 + T9, Id1_arr, T10),
  Id3_arr = setelement(1 + Id2_i, Id2_arr, Id2_tmp),
  sort_b5(Id2_i, Id3_arr, _times).

sort_b5(Id2_i, Id4_arr, _times) ->
  T13 = Id2_i + 1,
  Id3_i = T13,
  sort_b2(Id3_i, Id4_arr, _times).

sort_b7(Id1_arr, Id0_times) ->
  T17 = Id0_times + 1,
  {Id5_arr} = sort(T17, Id1_arr),
  sort_b8(Id5_arr).

sort_b8(Id6_arr) ->
  {Id6_arr}.


output(_arr) ->
  output_b1(_arr).

output_b1(_arr) ->
  Id1_i = 0,
  output_b2(Id1_i, _arr).

output_b2(Id2_i, _arr) ->
  case (Id2_i < 100) of
    false -> output_b4();
    true -> output_b3(Id2_i, _arr)
  end.

output_b3(Id2_i, Id0_arr) ->
  T2 = element(1 + Id2_i, Id0_arr),
  io:put_chars(integer_to_list(T2)),
  io:put_chars([10]),
  T5 = Id2_i + 1,
  Id3_i = T5,
  output_b2(Id3_i, Id0_arr).

output_b4() ->
  ok.


main(_arr) ->
  main_b1(_arr).

main_b1(Id0_arr) ->
  {Id1_arr} = generate(Id0_arr),
  {Id2_arr} = sort(0, Id1_arr),
  output(Id2_arr),
  {Id2_arr}.

entry_main() ->
  _arr = erlang:make_tuple(100, 0),
  main(_arr),
  ok.

