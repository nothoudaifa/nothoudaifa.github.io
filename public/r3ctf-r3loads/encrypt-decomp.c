unsigned __int64 __fastcall sub_1630(unsigned int *input, unsigned int *a2)
{
  v187 = __readfsqword(0x28u);
  first = *input;
  second = input[1];
  v168 = 0;
  v185 = 0uLL;
  memset(S, 0, 0x400uLL);
  S[0] = 1;
  S[1] = 2;
  S[2] = 3;
  S[3] = 4;
  S[4] = 5;
  for ( i = 0; i <= 255; ++i )
    S[i] = i;
  v170 = 0;
  v171 = 0;
  for ( i = 0; i <= 255; ++i )
  {
    v184 = S[i];
    v170 = (unsigned __int8)(v184 + v170 + a2[v171]);
    S[i] = S[v170];
    S[v170] = v184;
    if ( (unsigned int)++v171 > 3 )
      v171 = 0;
  }
  do
  {
    v172 = and_arg1_arg2(v168, 3LL, 4267278302LL);
    
    // start of static1
    
    v162 = mult_arg1_arg2;                      
    v159 = and_arg1_arg2;
    v154 = add_arg1_arg2(3879220815LL, 1303544047LL, 1673289997LL);
    v148 = or_arg1_arg2(119385303LL, 3231329148LL, 141954786LL);
    v145 = bitwise_not;
    v137 = and_arg1_arg2(1503872738LL, 978965565LL, 1470160614LL);
    v125 = mult_arg1_arg2(761654413LL, 3043887596LL, 4085454309LL);
    v120 = mult_arg1_arg2;
    v131 = xor_first_two(3333670936LL, 2147831941LL, 2217132274LL);
    v115 = add_arg1_arg2(4141055499LL, 1038626365LL, 1517838417LL);
    v112 = xor_first_two;
    v109 = add_arg1_arg2(2686461105LL, 3867813406LL, 686930351LL);
    v106 = xor_first_two(3259940977LL, 1820144441LL, 2738368048LL);
    v103 = mult_arg1_arg2;
    v2 = bitwise_not(2920661428LL, 408102610LL, 758347707LL);
    v3 = xor_first_two(2688446990LL, 3924691225LL, 3780184521LL);
    v4 = or_arg1_arg2;
    v5 = xor_first_two(715168552LL, 4203380807LL, 4042894304LL);
    v6 = and_arg1_arg2(3362982059LL, 1951226027LL, 2091265364LL);
    v7 = mult_arg1_arg2(2447727191LL, 4231433857LL, 2356928693LL);
    v8 = v4(v7, v6, v5);
    v9 = v103(v8, v3, v2);
    v10 = v112(v9, v106, v109);
    v11 = v120(v10, v115, v131);
    v12 = v145(v11, v125, v137);
    LODWORD(v159) = v159(v12, v148, v154);
    v155 = mult_arg1_arg2;
    v149 = integer_div_arg1_arg2;
    v146 = and_arg1_arg2;
    v138 = add_arg1_arg2(3879220815LL, 1303544047LL, 1673289997LL);
    v126 = or_arg1_arg2(119385303LL, 3231329148LL, 141954786LL);
    v121 = bitwise_not;
    v132 = and_arg1_arg2(1503872738LL, 978965565LL, 1470160614LL);
    v116 = mult_arg1_arg2(761654413LL, 3043887596LL, 4085454309LL);
    v113 = mult_arg1_arg2;
    v110 = xor_first_two(3333670936LL, 2147831941LL, 2217132274LL);
    v107 = add_arg1_arg2(4141055499LL, 1038626365LL, 1517838417LL);
    v104 = xor_first_two;
    v102 = add_arg1_arg2(2686461105LL, 3867813406LL, 686930351LL);
    v101 = xor_first_two(3259940977LL, 1820144441LL, 2738368048LL);
    v100 = mult_arg1_arg2;
    v13 = bitwise_not(2920661428LL, 408102610LL, 758347707LL);
    v14 = xor_first_two(2688446990LL, 3924691225LL, 3780184521LL);
    v15 = or_arg1_arg2;
    v16 = xor_first_two(715168552LL, 4203380807LL, 4042894304LL);
    v17 = and_arg1_arg2(3362982059LL, 1951226027LL, 2091265364LL);
    v18 = mult_arg1_arg2(2447727191LL, 4231433857LL, 2356928693LL);
    v19 = v15(v18, v17, v16);
    v20 = v100(v19, v14, v13);
    v21 = v104(v20, v101, v102);
    v22 = v113(v21, v107, v110);
    v23 = v121(v22, v116, v132);
    v24 = v146(v23, v126, v138);
    v25 = v149(v24, 7LL, 1501804614LL);
    v26 = v155(v25, 7LL, 3199106012LL);
    v27 = pow(2LL, (unsigned int)((_DWORD)v159 - v26 + 1), 3474473344LL);
    
    // end of static1
    
    
    v173 = v162(second, v27, 2318416522LL);
    
    // start of static2
    
    v163 = integer_div_arg1_arg2;               
    v28 = bitwise_not;
    LODWORD(v159) = or_arg1_arg2(4161192337LL, 897521730LL, 2969376499LL);
    LODWORD(v155) = or_arg1_arg2(2193193919LL, 1223334958LL, 3918650481LL);
    v29 = or_arg1_arg2;
    LODWORD(v149) = bitwise_not(1817983129LL, 994921689LL, 3011477667LL);
    LODWORD(v146) = bitwise_not(1066537828LL, 1283116571LL, 2740369546LL);
    v30 = and_arg1_arg2;
    v139 = mult_arg1_arg2(2890954343LL, 1376537582LL, 2181383581LL);
    v127 = and_arg1_arg2(1400330689LL, 1110986033LL, 4131805349LL);
    v122 = and_arg1_arg2;
    v31 = and_arg1_arg2(2171261435LL, 146029695LL, 3613482069LL);
    v32 = add_arg1_arg2(1785636003LL, 1488736597LL, 1361352486LL);
    v33 = mult_arg1_arg2(2535233854LL, 167541471LL, 1730975460LL);
    v34 = v122(v33, v32, v31);
    v35 = v30(v34, v127, v139);
    v36 = v29(v35, (unsigned int)v146, (unsigned int)v149);
    LODWORD(v30) = v28(v36, (unsigned int)v155, (unsigned int)v159);
    v160 = mult_arg1_arg2;
    v156 = integer_div_arg1_arg2;
    v150 = bitwise_not;
    LODWORD(v146) = or_arg1_arg2(4161192337LL, 897521730LL, 2969376499LL);
    v140 = or_arg1_arg2(2193193919LL, 1223334958LL, 3918650481LL);
    v128 = or_arg1_arg2;
    LODWORD(v122) = bitwise_not(1817983129LL, 994921689LL, 3011477667LL);
    v133 = bitwise_not(1066537828LL, 1283116571LL, 2740369546LL);
    v117 = and_arg1_arg2;
    LODWORD(v113) = mult_arg1_arg2(2890954343LL, 1376537582LL, 2181383581LL);
    LODWORD(v29) = and_arg1_arg2(1400330689LL, 1110986033LL, 4131805349LL);
    v37 = and_arg1_arg2;
    v38 = and_arg1_arg2(2171261435LL, 146029695LL, 3613482069LL);
    v39 = add_arg1_arg2(1785636003LL, 1488736597LL, 1361352486LL);
    v40 = mult_arg1_arg2(2535233854LL, 167541471LL, 1730975460LL);
    v41 = v37(v40, v39, v38);
    v42 = v117(v41, (unsigned int)v29, (unsigned int)v113);
    v43 = v128(v42, v133, (unsigned int)v122);
    v44 = v150(v43, v140, (unsigned int)v146);
    v45 = v156(v44, 7LL, 314984731LL);
    v46 = v160(v45, 7LL, 3158207403LL);
    v47 = pow(2LL, (unsigned int)((_DWORD)v30 - v46 + 1), 1008871084LL);
    
    // end of static2
    
    v174 = v163(second, v47, 197348412LL);
    v175 = read_integer_at_offset(S, v172, 447433706LL);
    v175 = add_arg1_arg2(v175, v168, 324602339LL);
    v176 = xor_first_two(v173, v174, 3329644733LL);
    v176 = add_arg1_arg2(v176, second, 1639485244LL);
    v48 = (void (__fastcall *)(unsigned int *, _QWORD, __int64))add_at_arg1_arg2;
    v49 = or_arg1_arg2;
    v50 = and_arg1_arg2;
    v51 = bitwise_not(v175, 653621050LL, 1423983245LL);
    LODWORD(v29) = v50(v51, v176, 1027126639LL);
    v52 = and_arg1_arg2;
    v53 = bitwise_not(v176, 3065730485LL, 1912144072LL);
    v54 = v52(v53, v175, 947070206LL);
    v55 = v49(v54, (unsigned int)v29, 1647839532LL);
    v48(&first, v55, 3300199018LL);
    
    // start of static3
    
    v164 = integer_div_arg1_arg2;               
    v56 = and_arg1_arg2;
    LODWORD(v160) = or_arg1_arg2(861961045LL, 3729566995LL, 1481462251LL);
    LODWORD(v156) = or_arg1_arg2(1015811113LL, 2892287594LL, 442448913LL);
    v151 = and_arg1_arg2;
    LODWORD(v146) = bitwise_not(2196330534LL, 253421437LL, 4099738111LL);
    v141 = bitwise_not(4269575248LL, 3279378526LL, 2915436928LL);
    v129 = mult_arg1_arg2;
    LODWORD(v122) = mult_arg1_arg2(3945310850LL, 2125372249LL, 640191863LL);
    v134 = xor_first_two(3731616660LL, 226513103LL, 2241794849LL);
    v118 = add_arg1_arg2;
    LODWORD(v113) = or_arg1_arg2(2297915302LL, 1861862940LL, 3719301326LL);
    LODWORD(v29) = xor_first_two(2214532763LL, 4266447871LL, 2078477805LL);
    v57 = add_arg1_arg2;
    LODWORD(v49) = xor_first_two(4257972909LL, 961609286LL, 557951297LL);
    LODWORD(v48) = add_arg1_arg2(4276531665LL, 513176722LL, 2816369635LL);
    v58 = add_arg1_arg2(2694453255LL, 2773612332LL, 2055293076LL);
    v59 = v57(v58, (unsigned int)v48, (unsigned int)v49);
    v60 = v118(v59, (unsigned int)v29, (unsigned int)v113);
    v61 = v129(v60, v134, (unsigned int)v122);
    v62 = v151(v61, v141, (unsigned int)v146);
    LODWORD(v160) = v56(v62, (unsigned int)v156, (unsigned int)v160);
    v157 = mult_arg1_arg2;
    v152 = integer_div_arg1_arg2;
    v147 = and_arg1_arg2;
    v142 = or_arg1_arg2(861961045LL, 3729566995LL, 1481462251LL);
    LODWORD(v129) = or_arg1_arg2(1015811113LL, 2892287594LL, 442448913LL);
    v123 = and_arg1_arg2;
    v135 = bitwise_not(2196330534LL, 253421437LL, 4099738111LL);
    LODWORD(v118) = bitwise_not(4269575248LL, 3279378526LL, 2915436928LL);
    v114 = mult_arg1_arg2;
    v111 = mult_arg1_arg2(3945310850LL, 2125372249LL, 640191863LL);
    v108 = xor_first_two(3731616660LL, 226513103LL, 2241794849LL);
    v105 = add_arg1_arg2;
    LODWORD(v56) = or_arg1_arg2(2297915302LL, 1861862940LL, 3719301326LL);
    LODWORD(v29) = xor_first_two(2214532763LL, 4266447871LL, 2078477805LL);
    v63 = add_arg1_arg2;
    LODWORD(v49) = xor_first_two(4257972909LL, 961609286LL, 557951297LL);
    LODWORD(v48) = add_arg1_arg2(4276531665LL, 513176722LL, 2816369635LL);
    v64 = add_arg1_arg2(2694453255LL, 2773612332LL, 2055293076LL);
    v65 = v63(v64, (unsigned int)v48, (unsigned int)v49);
    v66 = v105(v65, (unsigned int)v29, (unsigned int)v56);
    v67 = v114(v66, v108, v111);
    v68 = v123(v67, (unsigned int)v118, v135);
    v69 = v147(v68, (unsigned int)v129, v142);
    v70 = v152(v69, 7LL, 3278750119LL);
    v71 = v157(v70, 7LL, 2227083366LL);
    v72 = pow(2LL, (unsigned int)((_DWORD)v160 - v71 + 1), 336464741LL);
    
    // end of static3
    
    v177 = v164(first, v72, 2481324780LL);
    
    // start of static4
    
    v165 = mult_arg1_arg2;                      
    v73 = bitwise_not;
    LODWORD(v160) = mult_arg1_arg2(748454888LL, 2385136313LL, 1546242347LL);
    LODWORD(v157) = mult_arg1_arg2(840257265LL, 914175242LL, 4105844533LL);
    v74 = add_arg1_arg2;
    LODWORD(v152) = or_arg1_arg2(4105116680LL, 428215568LL, 3893947801LL);
    LODWORD(v147) = add_arg1_arg2(1050111887LL, 1686078548LL, 3318984746LL);
    v75 = mult_arg1_arg2;
    v143 = add_arg1_arg2(3758740715LL, 2625692561LL, 2391327272LL);
    LODWORD(v129) = add_arg1_arg2(3866433077LL, 39691962LL, 1819467951LL);
    v124 = or_arg1_arg2;
    LODWORD(v49) = mult_arg1_arg2(2001992146LL, 1101967082LL, 2965024266LL);
    LODWORD(v48) = xor_first_two(3387452349LL, 2334297832LL, 3975767251LL);
    v76 = and_arg1_arg2(2824204296LL, 3303156400LL, 1728631218LL);
    v77 = v124(v76, (unsigned int)v48, (unsigned int)v49);
    v78 = v75(v77, (unsigned int)v129, v143);
    v79 = v74(v78, (unsigned int)v147, (unsigned int)v152);
    LODWORD(v75) = v73(v79, (unsigned int)v157, (unsigned int)v160);
    v161 = mult_arg1_arg2;
    v158 = integer_div_arg1_arg2;
    v153 = bitwise_not;
    LODWORD(v147) = mult_arg1_arg2(748454888LL, 2385136313LL, 1546242347LL);
    v144 = mult_arg1_arg2(840257265LL, 914175242LL, 4105844533LL);
    v130 = add_arg1_arg2;
    LODWORD(v124) = or_arg1_arg2(4105116680LL, 428215568LL, 3893947801LL);
    v136 = add_arg1_arg2(1050111887LL, 1686078548LL, 3318984746LL);
    v119 = mult_arg1_arg2;
    LODWORD(v114) = add_arg1_arg2(3758740715LL, 2625692561LL, 2391327272LL);
    LODWORD(v74) = add_arg1_arg2(3866433077LL, 39691962LL, 1819467951LL);
    v80 = or_arg1_arg2;
    LODWORD(v49) = mult_arg1_arg2(2001992146LL, 1101967082LL, 2965024266LL);
    LODWORD(v48) = xor_first_two(3387452349LL, 2334297832LL, 3975767251LL);
    v81 = and_arg1_arg2(2824204296LL, 3303156400LL, 1728631218LL);
    v82 = v80(v81, (unsigned int)v48, (unsigned int)v49);
    v83 = v119(v82, (unsigned int)v74, (unsigned int)v114);
    v84 = v130(v83, v136, (unsigned int)v124);
    v85 = v153(v84, v144, (unsigned int)v147);
    v86 = v158(v85, 7LL, 3123289780LL);
    v87 = v161(v86, 7LL, 3107866936LL);
    v88 = pow(2LL, (unsigned int)((_DWORD)v75 - v87 + 1), 678071872LL);
    
    // end of static4
    
    
    v178 = v165(first, v88, 2809463385LL);
    v89 = or_arg1_arg2;
    v90 = and_arg1_arg2;
    v91 = bitwise_not(v177, 4059826110LL, 1422414646LL);
    LODWORD(v80) = v90(v91, v178, 3879744432LL);
    v92 = and_arg1_arg2;
    v93 = bitwise_not(v178, 4049347960LL, 1935114105LL);
    v94 = v92(v93, v177, 3415169072LL);
    v179 = v89(v94, (unsigned int)v80, 4120885783LL);
    add_at_arg1_arg2(&v168, 3284565212LL, 2416574660LL);
    v180 = add_arg1_arg2(v179, first, 1817052189LL);
    v95 = and_arg1_arg2;
    v96 = integer_div_arg1_arg2(v168, 2048LL, 2226869969LL);
    v181 = v95(255LL, v96, 472317835LL);
    v182 = read_integer_at_offset(S, v181, 1977943222LL);
    v183 = add_arg1_arg2(v182, v168, 4263146526LL);
    v97 = (void (__fastcall *)(unsigned int *, _QWORD, __int64))add_at_arg1_arg2;
    v98 = xor_first_two(v180, v183, 2191071771LL);
    v97(&second, v98, 1183580100LL);
    v185 += 1uLL;
  }
  while ( 0x478 >= v185 );
  *input = first;
  input[1] = second;
  return v187 - __readfsqword(0x28u);
}

