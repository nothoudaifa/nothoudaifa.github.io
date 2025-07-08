__int64 __fastcall main(int a1, char **a2, char **a3)
{
  v92 = __readfsqword(0x28u);
  
  // part 1
  handle = dlopen("./RRRRRRRRRRRRRRRRRRRR3RR33R3333RR.so", 1);
  if ( !handle )
    exit(1);
  dlerror();
  qword_7038 = (__int64 (__fastcall *)(_QWORD, _QWORD, _QWORD))dlsym(handle, "RRRRRRRRRRRRRRRRRRRR3RR33R3333R3");
  if ( dlerror() )
    exit(2);
  handlea = dlopen("./RRRRRRRRRRRRRRRRRRR3RR3RR333RR33.so", 1);
  if ( !handlea )
    exit(1);
  dlerror();
  qword_7028 = (__int64)dlsym(handlea, "RRRRRRRRRRRRRRRRRRR3RR3RR333R3RR");
  if ( dlerror() )
    exit(2);
  handleb = dlopen("./RRRRRRRRRRRRRRRRRRRR33333RRRRR33.so", 1);
  if ( !handleb )
    exit(1);
  dlerror();
  qword_7060 = (__int64)dlsym(handleb, "RRRRRRRRRRRRRRRRRRRR33333RRRR3RR");
  if ( dlerror() )
    exit(2);
  handlec = dlopen("./RRRRRRRRRRRRRRRRRRRRRRRR3333R333.so", 1);
  if ( !handlec )
    exit(1);
  dlerror();
  qword_7058 = (__int64 (__fastcall *)(_QWORD, _QWORD, _QWORD))dlsym(handlec, "RRRRRRRRRRRRRRRRRRRRRRRR3R3333R3");
  if ( dlerror() )
    exit(2);
  handled = dlopen("./RRRRRRRRRRRRRRRRRRRRR333RR33R3R3.so", 1);
  if ( !handled )
    exit(1);
  dlerror();
  qword_7030 = (__int64 (__fastcall *)(_QWORD, _QWORD, _QWORD))dlsym(handled, "RRRRRRRRRRRRRRRRRRRRR333RR33R33R");
  if ( dlerror() )
    exit(2);
  handlee = dlopen("./RRRRRRRRRRRRRRRRRRR33RRR33R3RRRR.so", 1);
  if ( !handlee )
    exit(1);
  dlerror();
  unk_7048 = dlsym(handlee, "RRRRRRRRRRRRRRRRRRR33RRR33R3RRR3");
  if ( dlerror() )
    exit(2);
  handlef = dlopen("./RRRRRRRRRRRRRRRRRRRRRRR333RR3RR3.so", 1);
  if ( !handlef )
    exit(1);
  dlerror();
  qword_7018 = (__int64 (__fastcall *)(_QWORD, _QWORD, _QWORD))dlsym(handlef, "RRRRRRRRRRRRRRRRRRRRRRR333RR3R3R");
  if ( dlerror() )
    exit(2);
  handleg = dlopen("./RRRRRRRRRRRRRRRRRRR3R3R3R33333R3.so", 1);
  if ( !handleg )
    exit(1);
  dlerror();
  qword_7050 = (__int64 (__fastcall *)(_QWORD, _QWORD, _QWORD))dlsym(handleg, "RRRRRRRRRRRRRRRRRRR3R3R3R333333R");
  if ( dlerror() )
    exit(2);
  handleh = dlopen("./RRRRRRRRRRRRRRRRRRRRR3RR333333R3.so", 1);
  if ( !handleh )
    exit(1);
  dlerror();
  qword_7020 = (__int64)dlsym(handleh, "RRRRRRRRRRRRRRRRRRRRR3RR33RRRRRR");
  if ( dlerror() )
    exit(2);
  handlei = dlopen("./RRRRRRRRRRRRRRRRRRRR33R33R3RRR33.so", 1);
  if ( !handlei )
    exit(1);
  dlerror();
  qword_7040 = (__int64 (__fastcall *)(_QWORD, _QWORD, _QWORD))dlsym(handlei, "RRRRRRRRRRRRRRRRRRRR33R33R3RR3RR");
  if ( dlerror() )
    exit(2);
   // part 2
  v78 = qword_7050;
  v74 = qword_7018(2519191252LL, 4076126875LL, 1218786772LL);
  v70 = qword_7030(1609595473LL, 3128484619LL, 4087579104LL);
  v68 = qword_7030;
  v65 = qword_7030(2690426595LL, 3364721587LL, 437895104LL);
  v61 = qword_7038(1283178194LL, 772598900LL, 4044443833LL);
  v56 = qword_7038;
  v53 = qword_7050(3011521025LL, 3896653472LL, 2969271853LL);
  v58 = qword_7050(2597383473LL, 1573495243LL, 1272799309LL);
  v51 = qword_7030;
  v49 = qword_7050(4124176520LL, 2851192274LL, 849819070LL);
  v47 = qword_7030(4204980766LL, 3303036188LL, 1300188808LL);
  v45 = qword_7030;
  v3 = qword_7040(1902249305LL, 480323615LL, 828915229LL);
  v4 = qword_7058(1051978252LL, 1556639948LL, 4057401978LL);
  v5 = qword_7050;
  v6 = qword_7040(1413845339LL, 1773044344LL, 1357571684LL);
  v7 = qword_7030(2424675220LL, 1451130298LL, 2243725890LL);
  v8 = qword_7038(2248010869LL, 4277979131LL, 4162855085LL);
  v9 = v5(v8, v7, v6);
  v10 = v45(v9, v4, v3);
  v11 = v51(v10, v47, v49);
  v12 = v56(v11, v58, v53);
  v13 = v68(v12, v61, v65);
  v91[0] = v78(v13, v70, v74);
  v79 = qword_7050;
  v75 = qword_7038(2252283101LL, 494639012LL, 2637468696LL);
  v71 = qword_7040(792239779LL, 4076212580LL, 2712479238LL);
  v69 = qword_7030;
  v66 = qword_7018(3865308284LL, 3805672778LL, 1080794858LL);
  v62 = qword_7038(1863963938LL, 1702626942LL, 2575324878LL);
  v57 = qword_7038;
  v54 = qword_7038(1357907110LL, 3832200346LL, 455731628LL);
  v59 = qword_7058(3455633054LL, 674668955LL, 3289048719LL);
  v52 = qword_7030;
  v50 = qword_7050(2970750527LL, 1480402215LL, 2703108150LL);
  v48 = qword_7058(2494397303LL, 3030882003LL, 1179715252LL);
  v46 = qword_7058;
  v14 = qword_7040(40620835LL, 3318471360LL, 2207650845LL);
  v15 = qword_7018(1006498893LL, 711788195LL, 1183453435LL);
  v16 = qword_7038;
  v17 = qword_7040(1672732083LL, 401989266LL, 3341448835LL);
  v18 = qword_7040(3541700768LL, 3304837126LL, 3494717532LL);
  v19 = qword_7038(619649146LL, 391288332LL, 3218547244LL);
  v20 = v16(v19, v18, v17);
  v21 = v46(v20, v15, v14);
  v22 = v52(v21, v48, v50);
  v23 = v57(v22, v59, v54);
  v24 = v69(v23, v62, v66);
  v91[3] = v79(v24, v71, v75);
  v25 = qword_7050;
  LODWORD(v79) = qword_7038(3506064512LL, 2110474017LL, 358014856LL);
  v76 = qword_7040(144378091LL, 2672728229LL, 2646192054LL);
  v26 = qword_7038;
  v72 = qword_7038(3765020811LL, 1633762065LL, 3301273113LL);
  LODWORD(v69) = qword_7050(711354758LL, 3844827092LL, 358041699LL);
  v67 = qword_7058;
  v63 = qword_7058(2021850418LL, 2909356192LL, 60369934LL);
  LODWORD(v57) = qword_7040(3929200604LL, 3942956373LL, 263092407LL);
  v55 = qword_7040;
  v60 = qword_7058(553441557LL, 546735616LL, 220887434LL);
  LODWORD(v52) = qword_7050(3064281611LL, 12666219LL, 1813867021LL);
  v27 = qword_7050;
  v28 = qword_7040(4176599994LL, 3799277863LL, 662859156LL);
  v29 = qword_7038(2174400086LL, 1471899898LL, 2172788515LL);
  v30 = qword_7050(2452834230LL, 3738633808LL, 2172492116LL);
  v31 = v27(v30, v29, v28);
  v32 = v55(v31, (unsigned int)v52, v60);
  v33 = v67(v32, (unsigned int)v57, v63);
  v34 = v26(v33, (unsigned int)v69, v72);
  v91[2] = v25(v34, v76, (unsigned int)v79);
  v35 = qword_7038;
  LODWORD(v79) = qword_7038(3376956997LL, 3487120587LL, 1389174544LL);
  v77 = qword_7018(264732934LL, 2848765287LL, 3377213004LL);
  v36 = qword_7050;
  v73 = qword_7038(759482441LL, 2318579817LL, 2362102760LL);
  LODWORD(v69) = qword_7030(570055162LL, 892712945LL, 1882563156LL);
  v37 = qword_7018;
  LODWORD(v67) = qword_7030(1702113315LL, 1339171625LL, 98089672LL);
  v64 = qword_7038(2670283717LL, 1353313077LL, 3169637536LL);
  v38 = qword_7030;
  LODWORD(v57) = qword_7050(4235657283LL, 433066747LL, 1625652836LL);
  v39 = qword_7050(2496388067LL, 258905451LL, 3454969537LL);
  v40 = qword_7038(1662268502LL, 2639566582LL, 1516828359LL);
  v41 = v38(v40, v39, (unsigned int)v57);
  v42 = v37(v41, v64, (unsigned int)v67);
  v43 = v36(v42, (unsigned int)v69, v73);
  v91[1] = v35(v43, v77, (unsigned int)v79);
  // part 3
  puts("Weclome to r3ctf 2025, can you beat me? 1/11423");
  puts("Input something: ");
  buf = 0LL;
  read(0, &buf, 8uLL);
  sub_1630(&buf, v91);
  if ( buf == 0xF06203EC3D2C5B74LL )
    puts("You win!");
  else
    puts("Try again!");
  return 0LL;
}
