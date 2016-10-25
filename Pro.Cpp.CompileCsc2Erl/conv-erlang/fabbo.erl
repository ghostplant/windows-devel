-module(fabbo).
-export([entry_main/0, fabbo/2, main/1]).

fabbo(_n, _ans) ->
  fabbo_b1(_n, _ans).

fabbo_b1(Id0_n, _ans) ->
  case (Id0_n > 2) of
    true -> fabbo_b3(Id0_n, _ans);
    false -> fabbo_b2()
  end.

fabbo_b2() ->
  Id1_ans = 1,
  fabbo_b6(Id1_ans).

fabbo_b3(_n, _ans) ->
  Id2_a = 1,
  Id2_b = 1,
  fabbo_b4(_n, _ans, Id2_a, Id2_b).

fabbo_b6(Id2_ans) ->
  {Id2_ans}.

fabbo_b4(Id2_n, Id3_ans, Id3_a, Id3_b) ->
  case (Id2_n < 3) of
    true -> fabbo_b6(Id3_ans);
    false -> fabbo_b5(Id2_n, Id3_a, Id3_b)
  end.

fabbo_b5(Id2_n, Id3_a, Id3_b) ->
  T6 = Id3_a + Id3_b,
  Id4_ans = T6,
  Id4_a = Id3_b,
  Id4_b = Id4_ans,
  T10 = Id2_n - 1,
  Id3_n = T10,
  fabbo_b4(Id3_n, Id4_ans, Id4_a, Id4_b).


main(_ans) ->
  main_b1(_ans).

main_b1(Id0_ans) ->
  T0 = list_to_integer(lists:delete(10,io:get_line([]))),
  Id1_n = T0,
  {Id1_ans} = fabbo(Id1_n, Id0_ans),
  io:put_chars(integer_to_list(Id1_ans)),
  io:put_chars([10]),
  {Id1_ans}.

entry_main() ->
  main(0),
  ok.

