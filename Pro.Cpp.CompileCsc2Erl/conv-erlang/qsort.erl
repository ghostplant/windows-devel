-module(qsort).
-export([entry_main/0, generate/1, qsort/3, output/1, main/1]).

generate(_arr) ->
  generate_b1(_arr).

generate_b1(_arr) ->
  Id1_i = 0,
  generate_b2(Id1_i, _arr).

generate_b2(Id2_i, Id1_arr) ->
  case (Id2_i < 100000) of
    false -> generate_b4(Id1_arr);
    true -> generate_b3(Id2_i, Id1_arr)
  end.

generate_b3(Id2_i, Id1_arr) ->
  T2 = 100 * Id2_i,
  T3 = T2 * Id2_i,
  T4 = T3 rem 31,
  Id2_arr = setelement(1 + Id2_i, Id1_arr, T4),
  T6 = Id2_i + 1,
  Id3_i = T6,
  generate_b2(Id3_i, Id2_arr).

generate_b4(_arr) ->
  {_arr}.


qsort(_r, _l, _arr) ->
  qsort_b1(_l, _r, _arr).

qsort_b1(Id0_l, Id0_r, Id0_arr) ->
  Id1_i = Id0_l,
  Id1_j = Id0_r,
  T2 = element(1 + Id1_i, Id0_arr),
  Id1_tmp = T2,
  qsort_b2(Id1_i, Id0_l, Id1_j, Id0_r, Id0_arr, Id1_tmp).

qsort_b2(Id1_i, _l, Id1_j, _r, Id1_arr, _tmp) ->
  case (Id1_i < Id1_j) of
    false -> qsort_b20(Id1_i, _l, _r, Id1_arr, _tmp);
    true -> qsort_b3(Id1_i, _l, Id1_j, _r, Id1_arr, _tmp)
  end.

qsort_b3(Id1_i, _l, Id1_j, _r, _arr, _tmp) ->
  Id2_flag = 0,
  case (Id1_i < Id1_j) of
    false -> qsort_b6(Id1_i, _l, Id1_j, _r, _arr, _tmp, Id2_flag);
    true -> qsort_b4(Id1_i, _l, Id1_j, _r, _arr, _tmp, Id2_flag)
  end.

qsort_b20(Id1_i, Id0_l, _r, Id1_arr, Id1_tmp) ->
  Id4_arr = setelement(1 + Id1_i, Id1_arr, Id1_tmp),
  T39 = Id1_i - 1,
  case (Id0_l < T39) of
    false -> qsort_b22(Id1_i, _r, Id4_arr);
    true -> qsort_b21(Id1_i, Id0_l, _r, Id4_arr)
  end.

qsort_b4(_i, _l, Id1_j, _r, Id1_arr, Id1_tmp, _flag) ->
  T7 = element(1 + Id1_j, Id1_arr),
  case (Id1_tmp > T7) of
    true -> qsort_b6(_i, _l, Id1_j, _r, Id1_arr, Id1_tmp, _flag);
    false -> qsort_b5(_i, _l, Id1_j, _r, Id1_arr, Id1_tmp)
  end.

qsort_b6(_i, _l, _j, _r, _arr, _tmp, Id3_flag) ->
  case (Id3_flag > 0) of
    false -> qsort_b11(_i, _l, _j, _r, _arr, _tmp);
    true -> qsort_b7(_i, _l, _j, _r, _arr, _tmp)
  end.

qsort_b21(Id1_i, Id0_l, _r, Id4_arr) ->
  T41 = Id1_i - 1,
  {Id5_arr} = qsort(T41, Id0_l, Id4_arr),
  qsort_b22(Id1_i, _r, Id5_arr).

qsort_b22(Id1_i, Id0_r, Id6_arr) ->
  T43 = Id1_i + 1,
  case (Id0_r > T43) of
    false -> qsort_b24(Id6_arr);
    true -> qsort_b23(Id1_i, Id0_r, Id6_arr)
  end.

qsort_b5(_i, _l, _j, _r, _arr, _tmp) ->
  Id3_flag = 1,
  qsort_b6(_i, _l, _j, _r, _arr, _tmp, Id3_flag).

qsort_b7(Id1_i, _l, Id1_j, _r, _arr, _tmp) ->
  T11 = Id1_j - 1,
  Id2_j = T11,
  Id4_flag = 0,
  case (Id1_i < Id2_j) of
    false -> qsort_b10(Id1_i, _l, Id2_j, _r, _arr, _tmp, Id4_flag);
    true -> qsort_b8(Id1_i, _l, Id2_j, _r, _arr, _tmp, Id4_flag)
  end.

qsort_b11(Id1_i, _l, Id1_j, _r, Id1_arr, _tmp) ->
  T19 = element(1 + Id1_j, Id1_arr),
  Id2_arr = setelement(1 + Id1_i, Id1_arr, T19),
  Id6_flag = 0,
  case (Id1_i < Id1_j) of
    false -> qsort_b14(Id1_i, _l, Id1_j, _r, Id2_arr, _tmp, Id6_flag);
    true -> qsort_b12(Id1_i, _l, Id1_j, _r, Id2_arr, _tmp, Id6_flag)
  end.

qsort_b23(Id1_i, Id0_r, Id6_arr) ->
  T45 = Id1_i + 1,
  {Id7_arr} = qsort(Id0_r, T45, Id6_arr),
  qsort_b24(Id7_arr).

qsort_b24(Id8_arr) ->
  {Id8_arr}.

qsort_b8(_i, _l, Id2_j, _r, Id1_arr, Id1_tmp, _flag) ->
  T15 = element(1 + Id2_j, Id1_arr),
  case (Id1_tmp > T15) of
    true -> qsort_b10(_i, _l, Id2_j, _r, Id1_arr, Id1_tmp, _flag);
    false -> qsort_b9(_i, _l, Id2_j, _r, Id1_arr, Id1_tmp)
  end.

qsort_b10(_i, _l, _j, _r, _arr, _tmp, _flag) ->
  qsort_b6(_i, _l, _j, _r, _arr, _tmp, _flag).

qsort_b12(Id1_i, _l, _j, _r, Id2_arr, Id1_tmp, _flag) ->
  T23 = element(1 + Id1_i, Id2_arr),
  case (Id1_tmp < T23) of
    true -> qsort_b14(Id1_i, _l, _j, _r, Id2_arr, Id1_tmp, _flag);
    false -> qsort_b13(Id1_i, _l, _j, _r, Id2_arr, Id1_tmp)
  end.

qsort_b14(_i, _l, _j, _r, _arr, _tmp, Id7_flag) ->
  case (Id7_flag > 0) of
    false -> qsort_b19(_i, _l, _j, _r, _arr, _tmp);
    true -> qsort_b15(_i, _l, _j, _r, _arr, _tmp)
  end.

qsort_b9(_i, _l, _j, _r, _arr, _tmp) ->
  Id5_flag = 1,
  qsort_b10(_i, _l, _j, _r, _arr, _tmp, Id5_flag).

qsort_b13(_i, _l, _j, _r, _arr, _tmp) ->
  Id7_flag = 1,
  qsort_b14(_i, _l, _j, _r, _arr, _tmp, Id7_flag).

qsort_b15(Id1_i, _l, Id1_j, _r, _arr, _tmp) ->
  T27 = Id1_i + 1,
  Id2_i = T27,
  Id8_flag = 0,
  case (Id2_i < Id1_j) of
    false -> qsort_b18(Id2_i, _l, Id1_j, _r, _arr, _tmp, Id8_flag);
    true -> qsort_b16(Id2_i, _l, Id1_j, _r, _arr, _tmp, Id8_flag)
  end.

qsort_b19(Id1_i, _l, Id1_j, _r, Id2_arr, _tmp) ->
  T35 = element(1 + Id1_i, Id2_arr),
  Id3_arr = setelement(1 + Id1_j, Id2_arr, T35),
  qsort_b2(Id1_i, _l, Id1_j, _r, Id3_arr, _tmp).

qsort_b16(Id2_i, _l, _j, _r, Id2_arr, Id1_tmp, _flag) ->
  T31 = element(1 + Id2_i, Id2_arr),
  case (Id1_tmp < T31) of
    true -> qsort_b18(Id2_i, _l, _j, _r, Id2_arr, Id1_tmp, _flag);
    false -> qsort_b17(Id2_i, _l, _j, _r, Id2_arr, Id1_tmp)
  end.

qsort_b18(_i, _l, _j, _r, _arr, _tmp, _flag) ->
  qsort_b14(_i, _l, _j, _r, _arr, _tmp, _flag).

qsort_b17(_i, _l, _j, _r, _arr, _tmp) ->
  Id9_flag = 1,
  qsort_b18(_i, _l, _j, _r, _arr, _tmp, Id9_flag).


output(_arr) ->
  output_b1(_arr).

output_b1(_arr) ->
  Id1_i = 0,
  output_b2(Id1_i, _arr).

output_b2(Id2_i, _arr) ->
  case (Id2_i < 100000) of
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
  T1 = 100000 - 1,
  {Id2_arr} = qsort(T1, 0, Id1_arr),
  output(Id2_arr),
  {Id2_arr}.

entry_main() ->
  _arr = erlang:make_tuple(100000, 0),
  main(_arr),
  ok.

