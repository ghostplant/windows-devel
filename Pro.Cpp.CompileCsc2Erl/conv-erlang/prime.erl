-module(prime).
-export([entry_main/0, prime/1, main/0]).

prime(_x) ->
  prime_b1(_x).

prime_b1(Id0_x) ->
  case (Id0_x > 1) of
    true -> prime_b3(Id0_x);
    false -> prime_b2()
  end.

prime_b2() ->
  T1 = - 1,
  Id1_ans = T1,
  prime_b8(Id1_ans).

prime_b3(_x) ->
  Id3_ans = 1,
  Id2_i = 2,
  prime_b4(_x, Id3_ans, Id2_i).

prime_b8(Id2_ans) ->
  {Id2_ans}.

prime_b4(Id0_x, Id4_ans, Id3_i) ->
  T6 = Id3_i * Id3_i,
  case (T6 > Id0_x) of
    true -> prime_b8(Id4_ans);
    false -> prime_b5(Id0_x, Id4_ans, Id3_i)
  end.

prime_b5(Id0_x, _ans, Id3_i) ->
  T8 = Id0_x rem Id3_i,
  case (T8 == 0) of
    false -> prime_b7(Id0_x, _ans, Id3_i);
    true -> prime_b6(Id0_x)
  end.

prime_b6(Id0_x) ->
  T10 = - 1,
  Id5_ans = T10,
  Id4_i = Id0_x,
  prime_b7(Id0_x, Id5_ans, Id4_i).

prime_b7(_x, Id6_ans, Id5_i) ->
  T13 = Id5_i + 1,
  Id6_i = T13,
  prime_b4(_x, Id6_ans, Id6_i).


main() ->
  main_b1().

main_b1() ->
  T0 = list_to_integer(lists:delete(10,io:get_line([]))),
  Id1_n = T0,
  {Id1_ans} = prime(Id1_n),
  io:put_chars(integer_to_list(Id1_ans)),
  io:put_chars([10]),
  {Id1_ans}.

entry_main() ->
  main(),
  ok.

