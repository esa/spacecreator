#ifdef PEG
struct T_SRC {
	char *fl; int ln;
} T_SRC[NTRANS];

void
tr_2_src(int m, char *file, int ln)
{	T_SRC[m].fl = file;
	T_SRC[m].ln = ln;
}

void
putpeg(int n, int m)
{	printf("%5d	trans %4d ", m, n);
	printf("%s:%d\n",
		T_SRC[n].fl, T_SRC[n].ln);
}
#endif

void
settable(void)
{	Trans *T;
	Trans *settr(int, int, int, int, int, char *, int, int, int);

	trans = (Trans ***) emalloc(11*sizeof(Trans **));

	/* proctype 9: :init: */

	trans[9] = (Trans **) emalloc(297*sizeof(Trans *));

	T = trans[ 9][295] = settr(2041,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(2041,2,5,0,0,"ATOMIC", 1, 2500, 0);
	T = trans[ 9][5] = settr(1751,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1751,0,4,0,0,"sub-sequence", 1, 2500, 0);
/*->*/	trans[9][4]	= settr(1750,34,73,3,3,"D_STEP20161", 1, 2, 0);
	T = trans[ 9][73] = settr(1819,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1819,0,9,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 9][9] = settr(1755,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1755,0,6,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][6]	= settr(1752,2,8,4,4,"global_state.actuator1.sender = 6", 1, 2500, 0);
	T = trans[ 9][8] = settr(1754,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1754,0,7,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][7]	= settr(1753,2,72,5,0,"assert((((((((global_state.actuator1.sender==0)||(global_state.actuator1.sender==1))||(global_state.actuator1.sender==2))||(global_state.actuator1.sender==3))||(global_state.actuator1.sender==4))||(global_state.actuator1.sender==5))||(global_state.actuator1.sender==6)))", 1, 2500, 0);
	T = trans[ 9][72] = settr(1818,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1818,0,10,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][10]	= settr(1756,2,69,6,6,"transition_id = 0", 1, 2500, 0); /* m: 11 -> 0,69 */
	reached9[11] = 1;
	trans[9][11]	= settr(0,0,0,0,0,"transition_id = 0",0,0,0);
	trans[9][70]	= settr(1816,2,69,1,0,".(goto)", 1, 2500, 0);
	T = trans[9][69] = settr(1815,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(1815,2,66,0,0,"DO", 1, 2500, 0);
	T = trans[9][66] = settr(1812,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1812,2,12,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1812,2,14,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1812,2,18,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1812,2,38,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1812,2,52,0,0,"IF", 1, 2500, 0);
	trans[9][12]	= settr(1758,2,74,7,7,"((transition_id==-(1)))", 1, 2500, 0);
	trans[9][13]	= settr(1759,2,74,1,0,"goto :b27", 1, 2500, 0);
	trans[9][67]	= settr(1813,2,68,1,0,".(goto)", 1, 2500, 0);
	trans[9][14]	= settr(1760,2,68,8,8,"((transition_id==0))", 1, 2500, 0); /* m: 15 -> 68,0 */
	reached9[15] = 1;
	trans[9][15]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][16]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[9][17]	= settr(1763,2,68,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][18]	= settr(1764,2,20,9,9,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 9][20] = settr(1766,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1766,0,19,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][19]	= settr(1765,2,36,1,0,"(1)", 1, 2500, 0);
	T = trans[9][36] = settr(1782,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1782,2,21,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1782,2,27,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1782,2,33,0,0,"IF", 1, 2500, 0);
	trans[9][21]	= settr(1767,2,23,10,0,"((global_state.actuator1.cmd==2))", 1, 2500, 0);
	T = trans[ 9][23] = settr(1769,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1769,0,22,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][22]	= settr(1768,2,68,11,11,"Modemanager_actuatorTm1_channel!2", 1, 2500, 0); /* m: 24 -> 68,0 */
	reached9[24] = 1;
	trans[9][24]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][25]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[9][26]	= settr(1772,2,68,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][37]	= settr(1783,2,68,1,0,".(goto)", 1, 2500, 0);
	trans[9][27]	= settr(1773,2,29,12,0,"((global_state.actuator1.cmd==1))", 1, 2500, 0);
	T = trans[ 9][29] = settr(1775,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1775,0,28,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][28]	= settr(1774,2,68,13,13,"Modemanager_actuatorTm1_channel!1", 1, 2500, 0); /* m: 30 -> 68,0 */
	reached9[30] = 1;
	trans[9][30]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][31]	= settr(0,0,0,0,0,"global_state.actuator1.state = 1",0,0,0);
	trans[9][32]	= settr(1778,2,68,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][33]	= settr(1779,2,34,2,0,"else", 1, 2500, 0);
	trans[9][34]	= settr(1780,2,68,14,14,"transition_id = -(1)", 1, 2500, 0);
	trans[9][35]	= settr(1781,2,68,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][38]	= settr(1784,2,40,15,15,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 9][40] = settr(1786,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1786,0,39,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][39]	= settr(1785,2,50,1,0,"(1)", 1, 2500, 0);
	T = trans[9][50] = settr(1796,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1796,2,41,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1796,2,47,0,0,"IF", 1, 2500, 0);
	trans[9][41]	= settr(1787,2,43,16,0,"((global_state.actuator1.cmd==2))", 1, 2500, 0);
	T = trans[ 9][43] = settr(1789,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1789,0,42,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][42]	= settr(1788,2,68,17,17,"Modemanager_actuatorTm1_channel!2", 1, 2500, 0); /* m: 44 -> 68,0 */
	reached9[44] = 1;
	trans[9][44]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][45]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[9][46]	= settr(1792,2,68,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][51]	= settr(1797,2,68,1,0,".(goto)", 1, 2500, 0);
	trans[9][47]	= settr(1793,2,48,2,0,"else", 1, 2500, 0);
	trans[9][48]	= settr(1794,2,68,18,18,"transition_id = -(1)", 1, 2500, 0);
	trans[9][49]	= settr(1795,2,68,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][52]	= settr(1798,2,54,19,19,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 9][54] = settr(1800,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1800,0,53,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][53]	= settr(1799,2,64,1,0,"(1)", 1, 2500, 0);
	T = trans[9][64] = settr(1810,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1810,2,55,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1810,2,61,0,0,"IF", 1, 2500, 0);
	trans[9][55]	= settr(1801,2,57,20,0,"((global_state.actuator1.cmd==0))", 1, 2500, 0);
	T = trans[ 9][57] = settr(1803,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1803,0,56,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][56]	= settr(1802,2,68,21,21,"Modemanager_actuatorTm1_channel!0", 1, 2500, 0); /* m: 58 -> 68,0 */
	reached9[58] = 1;
	trans[9][58]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][59]	= settr(0,0,0,0,0,"global_state.actuator1.state = 0",0,0,0);
	trans[9][60]	= settr(1806,2,68,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][65]	= settr(1811,2,68,1,0,".(goto)", 1, 2500, 0);
	trans[9][61]	= settr(1807,2,62,2,0,"else", 1, 2500, 0);
	trans[9][62]	= settr(1808,2,68,22,22,"transition_id = -(1)", 1, 2500, 0);
	trans[9][63]	= settr(1809,2,68,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][68]	= settr(1814,2,69,1,0,"(1)", 1, 2500, 0);
	trans[9][71]	= settr(1817,2,74,1,0,"break", 1, 2500, 0);
	trans[9][74]	= settr(1820,2,142,23,23,"Actuator1_lock!1", 1, 2500, 0);
	T = trans[ 9][142] = settr(1888,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1888,0,78,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 9][78] = settr(1824,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1824,0,75,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][75]	= settr(1821,2,77,24,24,"global_state.actuator2.sender = 6", 1, 2500, 0);
	T = trans[ 9][77] = settr(1823,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1823,0,76,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][76]	= settr(1822,2,141,25,0,"assert((((((((global_state.actuator2.sender==0)||(global_state.actuator2.sender==1))||(global_state.actuator2.sender==2))||(global_state.actuator2.sender==3))||(global_state.actuator2.sender==4))||(global_state.actuator2.sender==5))||(global_state.actuator2.sender==6)))", 1, 2500, 0);
	T = trans[ 9][141] = settr(1887,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1887,0,79,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][79]	= settr(1825,2,138,26,26,"transition_id = 0", 1, 2500, 0); /* m: 80 -> 0,138 */
	reached9[80] = 1;
	trans[9][80]	= settr(0,0,0,0,0,"transition_id = 0",0,0,0);
	trans[9][139]	= settr(1885,2,138,1,0,".(goto)", 1, 2500, 0);
	T = trans[9][138] = settr(1884,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(1884,2,135,0,0,"DO", 1, 2500, 0);
	T = trans[9][135] = settr(1881,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1881,2,81,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1881,2,83,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1881,2,87,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1881,2,107,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1881,2,121,0,0,"IF", 1, 2500, 0);
	trans[9][81]	= settr(1827,2,143,27,27,"((transition_id==-(1)))", 1, 2500, 0);
	trans[9][82]	= settr(1828,2,143,1,0,"goto :b28", 1, 2500, 0);
	trans[9][136]	= settr(1882,2,137,1,0,".(goto)", 1, 2500, 0);
	trans[9][83]	= settr(1829,2,137,28,28,"((transition_id==0))", 1, 2500, 0); /* m: 84 -> 137,0 */
	reached9[84] = 1;
	trans[9][84]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][85]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[9][86]	= settr(1832,2,137,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][87]	= settr(1833,2,89,29,29,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 9][89] = settr(1835,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1835,0,88,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][88]	= settr(1834,2,105,1,0,"(1)", 1, 2500, 0);
	T = trans[9][105] = settr(1851,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1851,2,90,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1851,2,96,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1851,2,102,0,0,"IF", 1, 2500, 0);
	trans[9][90]	= settr(1836,2,92,30,0,"((global_state.actuator2.cmd==2))", 1, 2500, 0);
	T = trans[ 9][92] = settr(1838,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1838,0,91,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][91]	= settr(1837,2,137,31,31,"Modemanager_actuatorTm2_channel!2", 1, 2500, 0); /* m: 93 -> 137,0 */
	reached9[93] = 1;
	trans[9][93]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][94]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[9][95]	= settr(1841,2,137,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][106]	= settr(1852,2,137,1,0,".(goto)", 1, 2500, 0);
	trans[9][96]	= settr(1842,2,98,32,0,"((global_state.actuator2.cmd==1))", 1, 2500, 0);
	T = trans[ 9][98] = settr(1844,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1844,0,97,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][97]	= settr(1843,2,137,33,33,"Modemanager_actuatorTm2_channel!1", 1, 2500, 0); /* m: 99 -> 137,0 */
	reached9[99] = 1;
	trans[9][99]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][100]	= settr(0,0,0,0,0,"global_state.actuator2.state = 1",0,0,0);
	trans[9][101]	= settr(1847,2,137,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][102]	= settr(1848,2,103,2,0,"else", 1, 2500, 0);
	trans[9][103]	= settr(1849,2,137,34,34,"transition_id = -(1)", 1, 2500, 0);
	trans[9][104]	= settr(1850,2,137,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][107]	= settr(1853,2,109,35,35,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 9][109] = settr(1855,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1855,0,108,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][108]	= settr(1854,2,119,1,0,"(1)", 1, 2500, 0);
	T = trans[9][119] = settr(1865,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1865,2,110,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1865,2,116,0,0,"IF", 1, 2500, 0);
	trans[9][110]	= settr(1856,2,112,36,0,"((global_state.actuator2.cmd==2))", 1, 2500, 0);
	T = trans[ 9][112] = settr(1858,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1858,0,111,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][111]	= settr(1857,2,137,37,37,"Modemanager_actuatorTm2_channel!2", 1, 2500, 0); /* m: 113 -> 137,0 */
	reached9[113] = 1;
	trans[9][113]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][114]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[9][115]	= settr(1861,2,137,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][120]	= settr(1866,2,137,1,0,".(goto)", 1, 2500, 0);
	trans[9][116]	= settr(1862,2,117,2,0,"else", 1, 2500, 0);
	trans[9][117]	= settr(1863,2,137,38,38,"transition_id = -(1)", 1, 2500, 0);
	trans[9][118]	= settr(1864,2,137,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][121]	= settr(1867,2,123,39,39,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 9][123] = settr(1869,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1869,0,122,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][122]	= settr(1868,2,133,1,0,"(1)", 1, 2500, 0);
	T = trans[9][133] = settr(1879,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1879,2,124,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1879,2,130,0,0,"IF", 1, 2500, 0);
	trans[9][124]	= settr(1870,2,126,40,0,"((global_state.actuator2.cmd==0))", 1, 2500, 0);
	T = trans[ 9][126] = settr(1872,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1872,0,125,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][125]	= settr(1871,2,137,41,41,"Modemanager_actuatorTm2_channel!0", 1, 2500, 0); /* m: 127 -> 137,0 */
	reached9[127] = 1;
	trans[9][127]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][128]	= settr(0,0,0,0,0,"global_state.actuator2.state = 0",0,0,0);
	trans[9][129]	= settr(1875,2,137,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][134]	= settr(1880,2,137,1,0,".(goto)", 1, 2500, 0);
	trans[9][130]	= settr(1876,2,131,2,0,"else", 1, 2500, 0);
	trans[9][131]	= settr(1877,2,137,42,42,"transition_id = -(1)", 1, 2500, 0);
	trans[9][132]	= settr(1878,2,137,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][137]	= settr(1883,2,138,1,0,"(1)", 1, 2500, 0);
	trans[9][140]	= settr(1886,2,143,1,0,"break", 1, 2500, 0);
	trans[9][143]	= settr(1889,2,292,43,43,"Actuator2_lock!1", 1, 2500, 0);
	T = trans[ 9][292] = settr(2038,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2038,0,147,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 9][147] = settr(1893,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1893,0,144,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][144]	= settr(1890,2,146,44,44,"global_state.modemanager.sender = 6", 1, 2500, 0);
	T = trans[ 9][146] = settr(1892,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1892,0,145,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][145]	= settr(1891,2,291,45,0,"assert((((((((global_state.modemanager.sender==0)||(global_state.modemanager.sender==1))||(global_state.modemanager.sender==2))||(global_state.modemanager.sender==3))||(global_state.modemanager.sender==4))||(global_state.modemanager.sender==5))||(global_state.modemanager.sender==6)))", 1, 2500, 0);
	T = trans[ 9][291] = settr(2037,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2037,0,148,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][148]	= settr(1894,2,288,46,46,"transition_id = 0", 1, 2500, 0); /* m: 149 -> 0,288 */
	reached9[149] = 1;
	trans[9][149]	= settr(0,0,0,0,0,"transition_id = 0",0,0,0);
	trans[9][289]	= settr(2035,2,288,1,0,".(goto)", 1, 2500, 0);
	T = trans[9][288] = settr(2034,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(2034,2,285,0,0,"DO", 1, 2500, 0);
	T = trans[9][285] = settr(2031,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2031,2,150,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2031,2,152,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2031,2,156,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2031,2,198,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2031,2,232,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2031,2,253,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2031,2,257,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2031,2,269,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2031,2,273,0,0,"IF", 1, 2500, 0);
	trans[9][150]	= settr(1896,2,293,47,47,"((transition_id==-(1)))", 1, 2500, 0);
	trans[9][151]	= settr(1897,2,293,1,0,"goto :b29", 1, 2500, 0);
	trans[9][286]	= settr(2032,2,287,1,0,".(goto)", 1, 2500, 0);
	trans[9][152]	= settr(1898,2,287,48,48,"((transition_id==0))", 1, 2500, 0); /* m: 153 -> 287,0 */
	reached9[153] = 1;
	trans[9][153]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][154]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[9][155]	= settr(1901,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][156]	= settr(1902,2,158,49,49,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 9][158] = settr(1904,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1904,0,157,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][157]	= settr(1903,2,196,1,0,"(1)", 1, 2500, 0);
	T = trans[9][196] = settr(1942,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1942,2,159,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1942,2,175,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1942,2,193,0,0,"IF", 1, 2500, 0);
	trans[9][159]	= settr(1905,2,163,50,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 9][163] = settr(1909,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1909,0,160,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][160]	= settr(1906,2,162,51,51,"global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 9][162] = settr(1908,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1908,0,161,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][161]	= settr(1907,2,167,52,0,"assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 9][167] = settr(1913,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1913,0,164,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][164]	= settr(1910,2,166,53,53,"global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 9][166] = settr(1912,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1912,0,165,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][165]	= settr(1911,2,169,54,0,"assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))", 1, 2500, 0);
	T = trans[ 9][169] = settr(1915,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1915,0,168,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][168]	= settr(1914,2,171,55,55,"Actuator1_actuatorTc_channel!0", 1, 2500, 0);
	T = trans[ 9][171] = settr(1917,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1917,0,170,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][170]	= settr(1916,2,287,56,56,"Actuator2_actuatorTc_channel!0", 1, 2500, 0); /* m: 172 -> 287,0 */
	reached9[172] = 1;
	trans[9][172]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][173]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[9][174]	= settr(1920,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][197]	= settr(1943,2,287,1,0,".(goto)", 1, 2500, 0);
	trans[9][175]	= settr(1921,2,188,57,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 9][188] = settr(1934,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1934,0,179,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 9][179] = settr(1925,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1925,0,176,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][176]	= settr(1922,2,178,58,58,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 9][178] = settr(1924,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1924,0,177,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][177]	= settr(1923,2,183,59,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 9][183] = settr(1929,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1929,0,180,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][180]	= settr(1926,2,182,60,60,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 9][182] = settr(1928,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1928,0,181,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][181]	= settr(1927,2,187,61,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 9][187] = settr(1933,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1933,0,184,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][184]	= settr(1930,2,186,62,62,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 9][186] = settr(1932,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1932,0,185,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][185]	= settr(1931,2,190,63,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 9][190] = settr(1936,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1936,0,189,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][189]	= settr(1935,2,287,64,64,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 191 -> 287,0 */
	reached9[191] = 1;
	trans[9][191]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][192]	= settr(1938,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][193]	= settr(1939,2,194,2,0,"else", 1, 2500, 0);
	trans[9][194]	= settr(1940,2,287,65,65,"transition_id = -(1)", 1, 2500, 0);
	trans[9][195]	= settr(1941,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][198]	= settr(1944,2,200,66,66,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 9][200] = settr(1946,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1946,0,199,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][199]	= settr(1945,2,230,1,0,"(1)", 1, 2500, 0);
	T = trans[9][230] = settr(1976,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1976,2,201,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1976,2,209,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1976,2,227,0,0,"IF", 1, 2500, 0);
	trans[9][201]	= settr(1947,2,203,67,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 9][203] = settr(1949,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1949,0,202,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][202]	= settr(1948,2,205,68,68,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[ 9][205] = settr(1951,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1951,0,204,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][204]	= settr(1950,2,287,69,69,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 206 -> 287,0 */
	reached9[206] = 1;
	trans[9][206]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][207]	= settr(0,0,0,0,0,"global_state.modemanager.state = 2",0,0,0);
	trans[9][208]	= settr(1954,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][231]	= settr(1977,2,287,1,0,".(goto)", 1, 2500, 0);
	trans[9][209]	= settr(1955,2,222,70,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 9][222] = settr(1968,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1968,0,213,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 9][213] = settr(1959,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1959,0,210,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][210]	= settr(1956,2,212,71,71,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 9][212] = settr(1958,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1958,0,211,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][211]	= settr(1957,2,217,72,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 9][217] = settr(1963,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1963,0,214,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][214]	= settr(1960,2,216,73,73,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 9][216] = settr(1962,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1962,0,215,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][215]	= settr(1961,2,221,74,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 9][221] = settr(1967,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1967,0,218,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][218]	= settr(1964,2,220,75,75,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 9][220] = settr(1966,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1966,0,219,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][219]	= settr(1965,2,224,76,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 9][224] = settr(1970,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1970,0,223,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][223]	= settr(1969,2,287,77,77,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 225 -> 287,0 */
	reached9[225] = 1;
	trans[9][225]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][226]	= settr(1972,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][227]	= settr(1973,2,228,2,0,"else", 1, 2500, 0);
	trans[9][228]	= settr(1974,2,287,78,78,"transition_id = -(1)", 1, 2500, 0);
	trans[9][229]	= settr(1975,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][232]	= settr(1978,2,234,79,79,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 9][234] = settr(1980,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1980,0,233,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][233]	= settr(1979,2,241,1,0,"(1)", 1, 2500, 0);
	T = trans[9][241] = settr(1987,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1987,2,235,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1987,2,238,0,0,"IF", 1, 2500, 0);
	trans[9][235]	= settr(1981,2,237,80,0,"((global_state.modemanager.r<global_state.modemanager.threshold1))", 1, 2500, 0);
	T = trans[ 9][237] = settr(1983,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1983,0,236,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][236]	= settr(1982,2,249,81,81,"Actuator1_actuatorTc_channel!1", 1, 2500, 0);
	trans[9][242]	= settr(1988,2,249,1,0,".(goto)", 1, 2500, 0);
	trans[9][238]	= settr(1984,2,240,2,0,"else", 1, 2500, 0);
	T = trans[ 9][240] = settr(1986,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1986,0,239,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][239]	= settr(1985,2,249,82,82,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[9][249] = settr(1995,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1995,2,243,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1995,2,246,0,0,"IF", 1, 2500, 0);
	trans[9][243]	= settr(1989,2,245,83,0,"((global_state.modemanager.r>global_state.modemanager.threshold2))", 1, 2500, 0);
	T = trans[ 9][245] = settr(1991,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1991,0,244,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][244]	= settr(1990,2,287,84,84,"Actuator2_actuatorTc_channel!1", 1, 2500, 0); /* m: 251 -> 287,0 */
	reached9[251] = 1;
	trans[9][250]	= settr(1996,2,251,1,0,".(goto)", 1, 2500, 0); /* m: 251 -> 0,287 */
	reached9[251] = 1;
	trans[9][246]	= settr(1992,2,248,2,0,"else", 1, 2500, 0);
	T = trans[ 9][248] = settr(1994,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1994,0,247,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][247]	= settr(1993,2,287,85,85,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 251 -> 287,0 */
	reached9[251] = 1;
	trans[9][251]	= settr(1997,2,287,86,86,"transition_id = -(1)", 1, 2500, 0);
	trans[9][252]	= settr(1998,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][253]	= settr(1999,2,255,87,87,"((transition_id==4))", 1, 2500, 0);
	T = trans[ 9][255] = settr(2001,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2001,0,254,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][254]	= settr(2000,2,267,1,0,"(1)", 1, 2500, 0);
	trans[9][256]	= settr(2002,2,267,1,0,"goto startcheck", 1, 2500, 0);
	trans[9][257]	= settr(2003,2,259,88,88,"((transition_id==5))", 1, 2500, 0);
	T = trans[ 9][259] = settr(2005,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2005,0,258,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][258]	= settr(2004,2,267,1,0,"(1)", 1, 2500, 0);
	T = trans[9][267] = settr(2013,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2013,2,260,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2013,2,263,0,0,"IF", 1, 2500, 0);
	trans[9][260]	= settr(2006,2,287,89,89,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))", 1, 2500, 0); /* m: 261 -> 287,0 */
	reached9[261] = 1;
	trans[9][261]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][262]	= settr(2008,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][268]	= settr(2014,2,287,1,0,".(goto)", 1, 2500, 0);
	trans[9][263]	= settr(2009,2,287,90,90,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))", 1, 2500, 0); /* m: 264 -> 287,0 */
	reached9[264] = 1;
	trans[9][264]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][265]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[9][266]	= settr(2012,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][269]	= settr(2015,2,271,91,91,"((transition_id==6))", 1, 2500, 0);
	T = trans[ 9][271] = settr(2017,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2017,0,270,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][270]	= settr(2016,2,283,1,0,"(1)", 1, 2500, 0);
	trans[9][272]	= settr(2018,2,283,1,0,"goto stopcheck", 1, 2500, 0);
	trans[9][273]	= settr(2019,2,275,92,92,"((transition_id==7))", 1, 2500, 0);
	T = trans[ 9][275] = settr(2021,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(2021,0,274,0,0,"sub-sequence", 1, 2500, 0);
	trans[9][274]	= settr(2020,2,283,1,0,"(1)", 1, 2500, 0);
	T = trans[9][283] = settr(2029,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(2029,2,276,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(2029,2,279,0,0,"IF", 1, 2500, 0);
	trans[9][276]	= settr(2022,2,287,93,93,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))", 1, 2500, 0); /* m: 277 -> 287,0 */
	reached9[277] = 1;
	trans[9][277]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][278]	= settr(2024,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][284]	= settr(2030,2,287,1,0,".(goto)", 1, 2500, 0);
	trans[9][279]	= settr(2025,2,287,94,94,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))", 1, 2500, 0); /* m: 280 -> 287,0 */
	reached9[280] = 1;
	trans[9][280]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[9][281]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[9][282]	= settr(2028,2,287,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[9][287]	= settr(2033,2,288,1,0,"(1)", 1, 2500, 0);
	trans[9][290]	= settr(2036,2,293,1,0,"break", 1, 2500, 0);
	trans[9][293]	= settr(2039,4,296,95,95,"Modemanager_lock!1", 1, 2500, 0); /* m: 294 -> 296,0 */
	reached9[294] = 1;
	trans[9][294]	= settr(0,0,0,0,0,"inited = 1",0,0,0);
	trans[9][296]	= settr(2042,0,0,96,96,"-end-", 0, 3500, 0);

	/* proctype 8: Sensor_reading */

	trans[8] = (Trans **) emalloc(29*sizeof(Trans *));

	trans[8][1]	= settr(1719,0,2,97,0,"(inited)", 1, 2, 0);
	trans[8][2]	= settr(1720,0,25,98,98,"value = 0", 0, 2, 0); /* m: 3 -> 0,25 */
	reached8[3] = 1;
	trans[8][3]	= settr(0,0,0,0,0,"inputVectorCounter = 0",0,0,0);
	trans[8][4]	= settr(0,0,0,0,0,"inputVectorCounter = 0",0,0,0);
	trans[8][26]	= settr(1744,0,25,1,0,".(goto)", 0, 2, 0);
	T = trans[8][25] = settr(1743,0,0,0,0,"DO", 0, 2, 0);
	T = T->nxt	= settr(1743,0,5,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(1743,0,23,0,0,"DO", 0, 2, 0);
	trans[8][5]	= settr(1723,0,21,99,0,"((inputVectorCounter<=3))", 0, 2, 0);
	T = trans[ 8][21] = settr(1739,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(1739,2,18,0,0,"ATOMIC", 1, 2, 0);
	T = trans[ 8][18] = settr(1736,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(1736,0,6,0,0,"sub-sequence", 1, 2, 0);
	trans[8][6]	= settr(1724,2,15,100,100,"DataItem_tmp = 0", 1, 2, 0);
	T = trans[8][15] = settr(1733,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(1733,2,14,0,0,"IF", 1, 2, 0);
	T = trans[ 8][14] = settr(1732,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(1732,0,7,0,0,"sub-sequence", 1, 2, 0);
	trans[8][7]	= settr(1725,2,11,101,101,"DataItem_tmp = 0", 1, 2, 0);
	trans[8][12]	= settr(1730,2,11,1,0,".(goto)", 1, 2, 0);
	T = trans[8][11] = settr(1729,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(1729,2,10,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(1729,2,8,0,0,"DO", 1, 2, 0);
	trans[8][10]	= settr(1728,2,20,102,102,"goto :b26", 1, 2, 0); /* m: 17 -> 0,20 */
	reached8[17] = 1;
	trans[8][8]	= settr(1726,2,11,103,103,"((DataItem_tmp<255))", 1, 2, 0); /* m: 9 -> 11,0 */
	reached8[9] = 1;
	trans[8][9]	= settr(0,0,0,0,0,"DataItem_tmp = (DataItem_tmp+1)",0,0,0);
	trans[8][13]	= settr(1731,2,17,1,0,"break", 1, 2, 0);
	trans[8][16]	= settr(1734,2,17,1,0,".(goto)", 1, 2, 0); /* m: 17 -> 0,20 */
	reached8[17] = 1;
	trans[8][17]	= settr(1735,2,20,104,104,"value = DataItem_tmp", 1, 2, 0);
	T = trans[ 8][20] = settr(1738,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(1738,0,19,0,0,"sub-sequence", 1, 2, 0);
	trans[8][19]	= settr(1737,0,25,105,105,"Modemanager_reading_channel!value", 1, 8, 0); /* m: 22 -> 25,0 */
	reached8[22] = 1;
	trans[8][22]	= settr(0,0,0,0,0,"inputVectorCounter = (inputVectorCounter+1)",0,0,0);
	trans[8][23]	= settr(1741,0,28,2,0,"else", 0, 2, 0);
	trans[8][24]	= settr(1742,0,28,1,0,"goto :b25", 0, 2, 0);
	trans[8][27]	= settr(1745,0,28,1,0,"break", 0, 2, 0);
	trans[8][28]	= settr(1746,0,0,106,106,"-end-", 0, 3500, 0);

	/* proctype 7: Modemanager_systemTc */

	trans[7] = (Trans **) emalloc(365*sizeof(Trans *));

	trans[7][1]	= settr(1355,0,361,107,0,"(inited)", 1, 2, 0);
	trans[7][362]	= settr(1716,0,361,1,0,".(goto)", 0, 2, 0);
	T = trans[7][361] = settr(1715,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(1715,0,360,0,0,"DO", 0, 2, 0);
	T = trans[ 7][360] = settr(1714,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(1714,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[7][2]	= settr(1356,2,3,108,0,"(nempty(Modemanager_systemTc_channel))", 1, 2500, 0);
	trans[7][3]	= settr(1357,2,357,109,109,"Modemanager_lock?_", 1, 2500, 0);
	T = trans[7][357] = settr(1711,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1711,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1711,2,355,0,0,"IF", 1, 2500, 0);
	trans[7][4]	= settr(1358,2,5,110,0,"(nempty(Modemanager_systemTc_channel))", 1, 2500, 0);
	trans[7][5]	= settr(1359,2,353,111,111,"Modemanager_systemTc_channel?Modemanager_systemtc_signal_parameter.data.goToIdle,Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1,Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2,Modemanager_systemtc_signal_parameter.data.doReport,Modemanager_systemtc_signal_parameter.selection", 1, 2500, 0); /* m: 6 -> 353,0 */
	reached7[6] = 1;
	trans[7][6]	= settr(0,0,0,0,0,"Modemanager_systemtc_channel_used = 1",0,0,0);
	T = trans[ 7][353] = settr(1707,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1707,0,351,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[7][351] = settr(1705,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1705,2,7,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1705,2,178,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1705,2,349,0,0,"IF", 1, 2500, 0);
	trans[7][7]	= settr(1361,2,33,112,0,"((global_state.modemanager.state==3))", 1, 2500, 0);
	T = trans[ 7][33] = settr(1387,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1387,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][8]	= settr(1362,2,31,113,113,"global_state.modemanager.stc.selection = Modemanager_systemtc_signal_parameter.selection", 1, 2500, 0);
	T = trans[7][31] = settr(1385,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1385,2,9,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1385,2,14,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1385,2,24,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1385,2,29,0,0,"IF", 1, 2500, 0);
	trans[7][9]	= settr(1363,2,13,114,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 7][13] = settr(1367,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1367,0,10,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][10]	= settr(1364,2,12,115,115,"global_state.modemanager.stc.data.goToIdle = Modemanager_systemtc_signal_parameter.data.goToIdle", 1, 2500, 0);
	T = trans[ 7][12] = settr(1366,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1366,0,11,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][11]	= settr(1365,2,177,116,0,"assert(((global_state.modemanager.stc.data.goToIdle>=0)&&(global_state.modemanager.stc.data.goToIdle<=0)))", 1, 2500, 0);
	trans[7][32]	= settr(1386,2,177,1,0,".(goto)", 1, 2500, 0);
	trans[7][14]	= settr(1368,2,23,117,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 7][23] = settr(1377,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1377,0,18,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][18] = settr(1372,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1372,0,15,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][15]	= settr(1369,2,17,118,118,"global_state.modemanager.stc.data.goToOperational.threshold1 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 7][17] = settr(1371,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1371,0,16,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][16]	= settr(1370,2,22,119,0,"assert(((global_state.modemanager.stc.data.goToOperational.threshold1>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 7][22] = settr(1376,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1376,0,19,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][19]	= settr(1373,2,21,120,120,"global_state.modemanager.stc.data.goToOperational.threshold2 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 7][21] = settr(1375,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1375,0,20,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][20]	= settr(1374,2,177,121,0,"assert(((global_state.modemanager.stc.data.goToOperational.threshold2>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold2<=255)))", 1, 2500, 0);
	trans[7][24]	= settr(1378,2,28,122,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 7][28] = settr(1382,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1382,0,25,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][25]	= settr(1379,2,27,123,123,"global_state.modemanager.stc.data.doReport = Modemanager_systemtc_signal_parameter.data.doReport", 1, 2500, 0);
	T = trans[ 7][27] = settr(1381,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1381,0,26,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][26]	= settr(1380,2,177,124,0,"assert(((global_state.modemanager.stc.data.doReport>=0)&&(global_state.modemanager.stc.data.doReport<=0)))", 1, 2500, 0);
	trans[7][29]	= settr(1383,2,30,2,0,"else", 1, 2500, 0);
	trans[7][30]	= settr(1384,2,177,1,0,"(1)", 1, 2500, 0);
	T = trans[ 7][177] = settr(1531,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1531,0,34,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][34]	= settr(1388,2,174,125,125,"transition_id = 0", 1, 2500, 0); /* m: 35 -> 0,174 */
	reached7[35] = 1;
	trans[7][35]	= settr(0,0,0,0,0,"transition_id = 1",0,0,0);
	trans[7][175]	= settr(1529,2,174,1,0,".(goto)", 1, 2500, 0);
	T = trans[7][174] = settr(1528,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(1528,2,171,0,0,"DO", 1, 2500, 0);
	T = trans[7][171] = settr(1525,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1525,2,36,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1525,2,38,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1525,2,42,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1525,2,84,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1525,2,118,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1525,2,139,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1525,2,143,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1525,2,155,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1525,2,159,0,0,"IF", 1, 2500, 0);
	trans[7][36]	= settr(1390,2,357,126,126,"((transition_id==-(1)))", 1, 2500, 0);
	trans[7][37]	= settr(1391,2,357,1,0,"goto :b23", 1, 2500, 0);
	trans[7][172]	= settr(1526,2,173,1,0,".(goto)", 1, 2500, 0);
	trans[7][38]	= settr(1392,2,173,127,127,"((transition_id==0))", 1, 2500, 0); /* m: 39 -> 173,0 */
	reached7[39] = 1;
	trans[7][39]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][40]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[7][41]	= settr(1395,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][42]	= settr(1396,2,44,128,128,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 7][44] = settr(1398,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1398,0,43,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][43]	= settr(1397,2,82,1,0,"(1)", 1, 2500, 0);
	T = trans[7][82] = settr(1436,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1436,2,45,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1436,2,61,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1436,2,79,0,0,"IF", 1, 2500, 0);
	trans[7][45]	= settr(1399,2,49,129,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 7][49] = settr(1403,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1403,0,46,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][46]	= settr(1400,2,48,130,130,"global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 7][48] = settr(1402,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1402,0,47,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][47]	= settr(1401,2,53,131,0,"assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 7][53] = settr(1407,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1407,0,50,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][50]	= settr(1404,2,52,132,132,"global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 7][52] = settr(1406,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1406,0,51,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][51]	= settr(1405,2,55,133,0,"assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))", 1, 2500, 0);
	T = trans[ 7][55] = settr(1409,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1409,0,54,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][54]	= settr(1408,2,57,134,134,"Actuator1_actuatorTc_channel!0", 1, 2500, 0);
	T = trans[ 7][57] = settr(1411,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1411,0,56,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][56]	= settr(1410,2,173,135,135,"Actuator2_actuatorTc_channel!0", 1, 2500, 0); /* m: 58 -> 173,0 */
	reached7[58] = 1;
	trans[7][58]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][59]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[7][60]	= settr(1414,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][83]	= settr(1437,2,173,1,0,".(goto)", 1, 2500, 0);
	trans[7][61]	= settr(1415,2,74,136,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 7][74] = settr(1428,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1428,0,65,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][65] = settr(1419,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1419,0,62,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][62]	= settr(1416,2,64,137,137,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 7][64] = settr(1418,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1418,0,63,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][63]	= settr(1417,2,69,138,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 7][69] = settr(1423,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1423,0,66,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][66]	= settr(1420,2,68,139,139,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 7][68] = settr(1422,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1422,0,67,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][67]	= settr(1421,2,73,140,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 7][73] = settr(1427,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1427,0,70,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][70]	= settr(1424,2,72,141,141,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 7][72] = settr(1426,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1426,0,71,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][71]	= settr(1425,2,76,142,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 7][76] = settr(1430,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1430,0,75,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][75]	= settr(1429,2,173,143,143,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 77 -> 173,0 */
	reached7[77] = 1;
	trans[7][77]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][78]	= settr(1432,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][79]	= settr(1433,2,80,2,0,"else", 1, 2500, 0);
	trans[7][80]	= settr(1434,2,173,144,144,"transition_id = -(1)", 1, 2500, 0);
	trans[7][81]	= settr(1435,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][84]	= settr(1438,2,86,145,145,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 7][86] = settr(1440,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1440,0,85,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][85]	= settr(1439,2,116,1,0,"(1)", 1, 2500, 0);
	T = trans[7][116] = settr(1470,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1470,2,87,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1470,2,95,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1470,2,113,0,0,"IF", 1, 2500, 0);
	trans[7][87]	= settr(1441,2,89,146,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 7][89] = settr(1443,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1443,0,88,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][88]	= settr(1442,2,91,147,147,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[ 7][91] = settr(1445,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1445,0,90,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][90]	= settr(1444,2,173,148,148,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 92 -> 173,0 */
	reached7[92] = 1;
	trans[7][92]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][93]	= settr(0,0,0,0,0,"global_state.modemanager.state = 2",0,0,0);
	trans[7][94]	= settr(1448,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][117]	= settr(1471,2,173,1,0,".(goto)", 1, 2500, 0);
	trans[7][95]	= settr(1449,2,108,149,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 7][108] = settr(1462,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1462,0,99,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][99] = settr(1453,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1453,0,96,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][96]	= settr(1450,2,98,150,150,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 7][98] = settr(1452,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1452,0,97,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][97]	= settr(1451,2,103,151,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 7][103] = settr(1457,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1457,0,100,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][100]	= settr(1454,2,102,152,152,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 7][102] = settr(1456,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1456,0,101,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][101]	= settr(1455,2,107,153,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 7][107] = settr(1461,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1461,0,104,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][104]	= settr(1458,2,106,154,154,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 7][106] = settr(1460,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1460,0,105,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][105]	= settr(1459,2,110,155,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 7][110] = settr(1464,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1464,0,109,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][109]	= settr(1463,2,173,156,156,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 111 -> 173,0 */
	reached7[111] = 1;
	trans[7][111]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][112]	= settr(1466,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][113]	= settr(1467,2,114,2,0,"else", 1, 2500, 0);
	trans[7][114]	= settr(1468,2,173,157,157,"transition_id = -(1)", 1, 2500, 0);
	trans[7][115]	= settr(1469,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][118]	= settr(1472,2,120,158,158,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 7][120] = settr(1474,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1474,0,119,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][119]	= settr(1473,2,127,1,0,"(1)", 1, 2500, 0);
	T = trans[7][127] = settr(1481,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1481,2,121,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1481,2,124,0,0,"IF", 1, 2500, 0);
	trans[7][121]	= settr(1475,2,123,159,0,"((global_state.modemanager.r<global_state.modemanager.threshold1))", 1, 2500, 0);
	T = trans[ 7][123] = settr(1477,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1477,0,122,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][122]	= settr(1476,2,135,160,160,"Actuator1_actuatorTc_channel!1", 1, 2500, 0);
	trans[7][128]	= settr(1482,2,135,1,0,".(goto)", 1, 2500, 0);
	trans[7][124]	= settr(1478,2,126,2,0,"else", 1, 2500, 0);
	T = trans[ 7][126] = settr(1480,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1480,0,125,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][125]	= settr(1479,2,135,161,161,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[7][135] = settr(1489,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1489,2,129,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1489,2,132,0,0,"IF", 1, 2500, 0);
	trans[7][129]	= settr(1483,2,131,162,0,"((global_state.modemanager.r>global_state.modemanager.threshold2))", 1, 2500, 0);
	T = trans[ 7][131] = settr(1485,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1485,0,130,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][130]	= settr(1484,2,173,163,163,"Actuator2_actuatorTc_channel!1", 1, 2500, 0); /* m: 137 -> 173,0 */
	reached7[137] = 1;
	trans[7][136]	= settr(1490,2,137,1,0,".(goto)", 1, 2500, 0); /* m: 137 -> 0,173 */
	reached7[137] = 1;
	trans[7][132]	= settr(1486,2,134,2,0,"else", 1, 2500, 0);
	T = trans[ 7][134] = settr(1488,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1488,0,133,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][133]	= settr(1487,2,173,164,164,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 137 -> 173,0 */
	reached7[137] = 1;
	trans[7][137]	= settr(1491,2,173,165,165,"transition_id = -(1)", 1, 2500, 0);
	trans[7][138]	= settr(1492,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][139]	= settr(1493,2,141,166,166,"((transition_id==4))", 1, 2500, 0);
	T = trans[ 7][141] = settr(1495,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1495,0,140,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][140]	= settr(1494,2,153,1,0,"(1)", 1, 2500, 0);
	trans[7][142]	= settr(1496,2,153,1,0,"goto startcheck", 1, 2500, 0);
	trans[7][143]	= settr(1497,2,145,167,167,"((transition_id==5))", 1, 2500, 0);
	T = trans[ 7][145] = settr(1499,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1499,0,144,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][144]	= settr(1498,2,153,1,0,"(1)", 1, 2500, 0);
	T = trans[7][153] = settr(1507,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1507,2,146,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1507,2,149,0,0,"IF", 1, 2500, 0);
	trans[7][146]	= settr(1500,2,173,168,168,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))", 1, 2500, 0); /* m: 147 -> 173,0 */
	reached7[147] = 1;
	trans[7][147]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][148]	= settr(1502,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][154]	= settr(1508,2,173,1,0,".(goto)", 1, 2500, 0);
	trans[7][149]	= settr(1503,2,173,169,169,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))", 1, 2500, 0); /* m: 150 -> 173,0 */
	reached7[150] = 1;
	trans[7][150]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][151]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[7][152]	= settr(1506,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][155]	= settr(1509,2,157,170,170,"((transition_id==6))", 1, 2500, 0);
	T = trans[ 7][157] = settr(1511,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1511,0,156,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][156]	= settr(1510,2,169,1,0,"(1)", 1, 2500, 0);
	trans[7][158]	= settr(1512,2,169,1,0,"goto stopcheck", 1, 2500, 0);
	trans[7][159]	= settr(1513,2,161,171,171,"((transition_id==7))", 1, 2500, 0);
	T = trans[ 7][161] = settr(1515,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1515,0,160,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][160]	= settr(1514,2,169,1,0,"(1)", 1, 2500, 0);
	T = trans[7][169] = settr(1523,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1523,2,162,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1523,2,165,0,0,"IF", 1, 2500, 0);
	trans[7][162]	= settr(1516,2,173,172,172,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))", 1, 2500, 0); /* m: 163 -> 173,0 */
	reached7[163] = 1;
	trans[7][163]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][164]	= settr(1518,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][170]	= settr(1524,2,173,1,0,".(goto)", 1, 2500, 0);
	trans[7][165]	= settr(1519,2,173,173,173,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))", 1, 2500, 0); /* m: 166 -> 173,0 */
	reached7[166] = 1;
	trans[7][166]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][167]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[7][168]	= settr(1522,2,173,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][173]	= settr(1527,2,174,1,0,"(1)", 1, 2500, 0);
	trans[7][176]	= settr(1530,2,357,1,0,"break", 1, 2500, 0);
	trans[7][352]	= settr(1706,2,357,1,0,".(goto)", 1, 2500, 0);
	trans[7][178]	= settr(1532,2,204,174,0,"((global_state.modemanager.state==1))", 1, 2500, 0);
	T = trans[ 7][204] = settr(1558,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1558,0,179,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][179]	= settr(1533,2,202,175,175,"global_state.modemanager.stc.selection = Modemanager_systemtc_signal_parameter.selection", 1, 2500, 0);
	T = trans[7][202] = settr(1556,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1556,2,180,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1556,2,185,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1556,2,195,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1556,2,200,0,0,"IF", 1, 2500, 0);
	trans[7][180]	= settr(1534,2,184,176,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 7][184] = settr(1538,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1538,0,181,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][181]	= settr(1535,2,183,177,177,"global_state.modemanager.stc.data.goToIdle = Modemanager_systemtc_signal_parameter.data.goToIdle", 1, 2500, 0);
	T = trans[ 7][183] = settr(1537,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1537,0,182,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][182]	= settr(1536,2,348,178,0,"assert(((global_state.modemanager.stc.data.goToIdle>=0)&&(global_state.modemanager.stc.data.goToIdle<=0)))", 1, 2500, 0);
	trans[7][203]	= settr(1557,2,348,1,0,".(goto)", 1, 2500, 0);
	trans[7][185]	= settr(1539,2,194,179,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 7][194] = settr(1548,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1548,0,189,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][189] = settr(1543,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1543,0,186,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][186]	= settr(1540,2,188,180,180,"global_state.modemanager.stc.data.goToOperational.threshold1 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 7][188] = settr(1542,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1542,0,187,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][187]	= settr(1541,2,193,181,0,"assert(((global_state.modemanager.stc.data.goToOperational.threshold1>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 7][193] = settr(1547,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1547,0,190,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][190]	= settr(1544,2,192,182,182,"global_state.modemanager.stc.data.goToOperational.threshold2 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 7][192] = settr(1546,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1546,0,191,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][191]	= settr(1545,2,348,183,0,"assert(((global_state.modemanager.stc.data.goToOperational.threshold2>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold2<=255)))", 1, 2500, 0);
	trans[7][195]	= settr(1549,2,199,184,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 7][199] = settr(1553,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1553,0,196,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][196]	= settr(1550,2,198,185,185,"global_state.modemanager.stc.data.doReport = Modemanager_systemtc_signal_parameter.data.doReport", 1, 2500, 0);
	T = trans[ 7][198] = settr(1552,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1552,0,197,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][197]	= settr(1551,2,348,186,0,"assert(((global_state.modemanager.stc.data.doReport>=0)&&(global_state.modemanager.stc.data.doReport<=0)))", 1, 2500, 0);
	trans[7][200]	= settr(1554,2,201,2,0,"else", 1, 2500, 0);
	trans[7][201]	= settr(1555,2,348,1,0,"(1)", 1, 2500, 0);
	T = trans[ 7][348] = settr(1702,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1702,0,205,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][205]	= settr(1559,2,345,187,187,"transition_id = 0", 1, 2500, 0); /* m: 206 -> 0,345 */
	reached7[206] = 1;
	trans[7][206]	= settr(0,0,0,0,0,"transition_id = 2",0,0,0);
	trans[7][346]	= settr(1700,2,345,1,0,".(goto)", 1, 2500, 0);
	T = trans[7][345] = settr(1699,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(1699,2,342,0,0,"DO", 1, 2500, 0);
	T = trans[7][342] = settr(1696,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1696,2,207,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1696,2,209,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1696,2,213,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1696,2,255,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1696,2,289,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1696,2,310,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1696,2,314,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1696,2,326,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1696,2,330,0,0,"IF", 1, 2500, 0);
	trans[7][207]	= settr(1561,2,357,188,188,"((transition_id==-(1)))", 1, 2500, 0);
	trans[7][208]	= settr(1562,2,357,1,0,"goto :b24", 1, 2500, 0);
	trans[7][343]	= settr(1697,2,344,1,0,".(goto)", 1, 2500, 0);
	trans[7][209]	= settr(1563,2,344,189,189,"((transition_id==0))", 1, 2500, 0); /* m: 210 -> 344,0 */
	reached7[210] = 1;
	trans[7][210]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][211]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[7][212]	= settr(1566,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][213]	= settr(1567,2,215,190,190,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 7][215] = settr(1569,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1569,0,214,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][214]	= settr(1568,2,253,1,0,"(1)", 1, 2500, 0);
	T = trans[7][253] = settr(1607,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1607,2,216,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1607,2,232,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1607,2,250,0,0,"IF", 1, 2500, 0);
	trans[7][216]	= settr(1570,2,220,191,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 7][220] = settr(1574,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1574,0,217,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][217]	= settr(1571,2,219,192,192,"global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 7][219] = settr(1573,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1573,0,218,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][218]	= settr(1572,2,224,193,0,"assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 7][224] = settr(1578,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1578,0,221,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][221]	= settr(1575,2,223,194,194,"global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 7][223] = settr(1577,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1577,0,222,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][222]	= settr(1576,2,226,195,0,"assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))", 1, 2500, 0);
	T = trans[ 7][226] = settr(1580,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1580,0,225,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][225]	= settr(1579,2,228,196,196,"Actuator1_actuatorTc_channel!0", 1, 2500, 0);
	T = trans[ 7][228] = settr(1582,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1582,0,227,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][227]	= settr(1581,2,344,197,197,"Actuator2_actuatorTc_channel!0", 1, 2500, 0); /* m: 229 -> 344,0 */
	reached7[229] = 1;
	trans[7][229]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][230]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[7][231]	= settr(1585,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][254]	= settr(1608,2,344,1,0,".(goto)", 1, 2500, 0);
	trans[7][232]	= settr(1586,2,245,198,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 7][245] = settr(1599,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1599,0,236,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][236] = settr(1590,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1590,0,233,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][233]	= settr(1587,2,235,199,199,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 7][235] = settr(1589,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1589,0,234,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][234]	= settr(1588,2,240,200,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 7][240] = settr(1594,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1594,0,237,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][237]	= settr(1591,2,239,201,201,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 7][239] = settr(1593,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1593,0,238,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][238]	= settr(1592,2,244,202,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 7][244] = settr(1598,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1598,0,241,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][241]	= settr(1595,2,243,203,203,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 7][243] = settr(1597,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1597,0,242,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][242]	= settr(1596,2,247,204,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 7][247] = settr(1601,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1601,0,246,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][246]	= settr(1600,2,344,205,205,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 248 -> 344,0 */
	reached7[248] = 1;
	trans[7][248]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][249]	= settr(1603,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][250]	= settr(1604,2,251,2,0,"else", 1, 2500, 0);
	trans[7][251]	= settr(1605,2,344,206,206,"transition_id = -(1)", 1, 2500, 0);
	trans[7][252]	= settr(1606,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][255]	= settr(1609,2,257,207,207,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 7][257] = settr(1611,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1611,0,256,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][256]	= settr(1610,2,287,1,0,"(1)", 1, 2500, 0);
	T = trans[7][287] = settr(1641,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1641,2,258,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1641,2,266,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1641,2,284,0,0,"IF", 1, 2500, 0);
	trans[7][258]	= settr(1612,2,260,208,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 7][260] = settr(1614,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1614,0,259,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][259]	= settr(1613,2,262,209,209,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[ 7][262] = settr(1616,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1616,0,261,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][261]	= settr(1615,2,344,210,210,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 263 -> 344,0 */
	reached7[263] = 1;
	trans[7][263]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][264]	= settr(0,0,0,0,0,"global_state.modemanager.state = 2",0,0,0);
	trans[7][265]	= settr(1619,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][288]	= settr(1642,2,344,1,0,".(goto)", 1, 2500, 0);
	trans[7][266]	= settr(1620,2,279,211,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 7][279] = settr(1633,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1633,0,270,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 7][270] = settr(1624,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1624,0,267,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][267]	= settr(1621,2,269,212,212,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 7][269] = settr(1623,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1623,0,268,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][268]	= settr(1622,2,274,213,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 7][274] = settr(1628,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1628,0,271,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][271]	= settr(1625,2,273,214,214,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 7][273] = settr(1627,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1627,0,272,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][272]	= settr(1626,2,278,215,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 7][278] = settr(1632,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1632,0,275,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][275]	= settr(1629,2,277,216,216,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 7][277] = settr(1631,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1631,0,276,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][276]	= settr(1630,2,281,217,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 7][281] = settr(1635,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1635,0,280,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][280]	= settr(1634,2,344,218,218,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 282 -> 344,0 */
	reached7[282] = 1;
	trans[7][282]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][283]	= settr(1637,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][284]	= settr(1638,2,285,2,0,"else", 1, 2500, 0);
	trans[7][285]	= settr(1639,2,344,219,219,"transition_id = -(1)", 1, 2500, 0);
	trans[7][286]	= settr(1640,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][289]	= settr(1643,2,291,220,220,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 7][291] = settr(1645,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1645,0,290,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][290]	= settr(1644,2,298,1,0,"(1)", 1, 2500, 0);
	T = trans[7][298] = settr(1652,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1652,2,292,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1652,2,295,0,0,"IF", 1, 2500, 0);
	trans[7][292]	= settr(1646,2,294,221,0,"((global_state.modemanager.r<global_state.modemanager.threshold1))", 1, 2500, 0);
	T = trans[ 7][294] = settr(1648,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1648,0,293,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][293]	= settr(1647,2,306,222,222,"Actuator1_actuatorTc_channel!1", 1, 2500, 0);
	trans[7][299]	= settr(1653,2,306,1,0,".(goto)", 1, 2500, 0);
	trans[7][295]	= settr(1649,2,297,2,0,"else", 1, 2500, 0);
	T = trans[ 7][297] = settr(1651,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1651,0,296,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][296]	= settr(1650,2,306,223,223,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[7][306] = settr(1660,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1660,2,300,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1660,2,303,0,0,"IF", 1, 2500, 0);
	trans[7][300]	= settr(1654,2,302,224,0,"((global_state.modemanager.r>global_state.modemanager.threshold2))", 1, 2500, 0);
	T = trans[ 7][302] = settr(1656,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1656,0,301,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][301]	= settr(1655,2,344,225,225,"Actuator2_actuatorTc_channel!1", 1, 2500, 0); /* m: 308 -> 344,0 */
	reached7[308] = 1;
	trans[7][307]	= settr(1661,2,308,1,0,".(goto)", 1, 2500, 0); /* m: 308 -> 0,344 */
	reached7[308] = 1;
	trans[7][303]	= settr(1657,2,305,2,0,"else", 1, 2500, 0);
	T = trans[ 7][305] = settr(1659,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1659,0,304,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][304]	= settr(1658,2,344,226,226,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 308 -> 344,0 */
	reached7[308] = 1;
	trans[7][308]	= settr(1662,2,344,227,227,"transition_id = -(1)", 1, 2500, 0);
	trans[7][309]	= settr(1663,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][310]	= settr(1664,2,312,228,228,"((transition_id==4))", 1, 2500, 0);
	T = trans[ 7][312] = settr(1666,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1666,0,311,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][311]	= settr(1665,2,324,1,0,"(1)", 1, 2500, 0);
	trans[7][313]	= settr(1667,2,324,1,0,"goto startcheck", 1, 2500, 0);
	trans[7][314]	= settr(1668,2,316,229,229,"((transition_id==5))", 1, 2500, 0);
	T = trans[ 7][316] = settr(1670,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1670,0,315,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][315]	= settr(1669,2,324,1,0,"(1)", 1, 2500, 0);
	T = trans[7][324] = settr(1678,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1678,2,317,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1678,2,320,0,0,"IF", 1, 2500, 0);
	trans[7][317]	= settr(1671,2,344,230,230,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))", 1, 2500, 0); /* m: 318 -> 344,0 */
	reached7[318] = 1;
	trans[7][318]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][319]	= settr(1673,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][325]	= settr(1679,2,344,1,0,".(goto)", 1, 2500, 0);
	trans[7][320]	= settr(1674,2,344,231,231,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))", 1, 2500, 0); /* m: 321 -> 344,0 */
	reached7[321] = 1;
	trans[7][321]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][322]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[7][323]	= settr(1677,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][326]	= settr(1680,2,328,232,232,"((transition_id==6))", 1, 2500, 0);
	T = trans[ 7][328] = settr(1682,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1682,0,327,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][327]	= settr(1681,2,340,1,0,"(1)", 1, 2500, 0);
	trans[7][329]	= settr(1683,2,340,1,0,"goto stopcheck", 1, 2500, 0);
	trans[7][330]	= settr(1684,2,332,233,233,"((transition_id==7))", 1, 2500, 0);
	T = trans[ 7][332] = settr(1686,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1686,0,331,0,0,"sub-sequence", 1, 2500, 0);
	trans[7][331]	= settr(1685,2,340,1,0,"(1)", 1, 2500, 0);
	T = trans[7][340] = settr(1694,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1694,2,333,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1694,2,336,0,0,"IF", 1, 2500, 0);
	trans[7][333]	= settr(1687,2,344,234,234,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))", 1, 2500, 0); /* m: 334 -> 344,0 */
	reached7[334] = 1;
	trans[7][334]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][335]	= settr(1689,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][341]	= settr(1695,2,344,1,0,".(goto)", 1, 2500, 0);
	trans[7][336]	= settr(1690,2,344,235,235,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))", 1, 2500, 0); /* m: 337 -> 344,0 */
	reached7[337] = 1;
	trans[7][337]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[7][338]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[7][339]	= settr(1693,2,344,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[7][344]	= settr(1698,2,345,1,0,"(1)", 1, 2500, 0);
	trans[7][347]	= settr(1701,2,357,1,0,"break", 1, 2500, 0);
	trans[7][349]	= settr(1703,2,350,2,0,"else", 1, 2500, 0);
	trans[7][350]	= settr(1704,2,357,1,0,"(1)", 1, 2500, 0);
	trans[7][354]	= settr(1708,2,357,1,0,"goto Modemanager_systemtc_loop", 1, 2500, 0);
	trans[7][358]	= settr(1712,2,359,1,0,".(goto)", 1, 2500, 0);
	trans[7][355]	= settr(1709,2,356,236,0,"(empty(Modemanager_systemTc_channel))", 1, 2500, 0);
	trans[7][356]	= settr(1710,2,359,237,237,"(1)", 1, 2500, 0); /* m: 358 -> 359,0 */
	reached7[358] = 1;
	trans[7][359]	= settr(1713,0,361,238,238,"Modemanager_lock!1", 1, 2500, 0);
	trans[7][363]	= settr(1717,0,364,1,0,"break", 0, 2, 0);
	trans[7][364]	= settr(1718,0,0,239,239,"-end-", 0, 3500, 0);

	/* proctype 6: Modemanager_reading */

	trans[6] = (Trans **) emalloc(172*sizeof(Trans *));

	trans[6][1]	= settr(1184,0,168,240,0,"(inited)", 1, 2, 0);
	trans[6][169]	= settr(1352,0,168,1,0,".(goto)", 0, 2, 0);
	T = trans[6][168] = settr(1351,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(1351,0,167,0,0,"DO", 0, 2, 0);
	T = trans[ 6][167] = settr(1350,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(1350,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[6][2]	= settr(1185,2,3,241,0,"(nempty(Modemanager_reading_channel))", 1, 2500, 0);
	trans[6][3]	= settr(1186,2,164,242,242,"Modemanager_lock?_", 1, 2500, 0);
	T = trans[6][164] = settr(1347,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1347,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1347,2,162,0,0,"IF", 1, 2500, 0);
	trans[6][4]	= settr(1187,2,5,243,0,"(nempty(Modemanager_reading_channel))", 1, 2500, 0);
	trans[6][5]	= settr(1188,2,160,244,244,"Modemanager_reading_channel?Modemanager_reading_signal_parameter", 1, 2500, 0); /* m: 6 -> 160,0 */
	reached6[6] = 1;
	trans[6][6]	= settr(0,0,0,0,0,"Modemanager_reading_channel_used = 1",0,0,0);
	T = trans[ 6][160] = settr(1343,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1343,0,158,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[6][158] = settr(1341,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1341,2,7,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1341,2,156,0,0,"IF", 1, 2500, 0);
	trans[6][7]	= settr(1190,2,11,245,0,"((global_state.modemanager.state==1))", 1, 2500, 0);
	T = trans[ 6][11] = settr(1194,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1194,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][8]	= settr(1191,2,10,246,246,"global_state.modemanager.r = Modemanager_reading_signal_parameter", 1, 2500, 0);
	T = trans[ 6][10] = settr(1193,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1193,0,9,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][9]	= settr(1192,2,155,247,0,"assert(((global_state.modemanager.r>=0)&&(global_state.modemanager.r<=255)))", 1, 2500, 0);
	T = trans[ 6][155] = settr(1338,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1338,0,12,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][12]	= settr(1195,2,152,248,248,"transition_id = 0", 1, 2500, 0); /* m: 13 -> 0,152 */
	reached6[13] = 1;
	trans[6][13]	= settr(0,0,0,0,0,"transition_id = 3",0,0,0);
	trans[6][153]	= settr(1336,2,152,1,0,".(goto)", 1, 2500, 0);
	T = trans[6][152] = settr(1335,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(1335,2,149,0,0,"DO", 1, 2500, 0);
	T = trans[6][149] = settr(1332,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1332,2,14,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1332,2,16,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1332,2,20,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1332,2,62,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1332,2,96,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1332,2,117,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1332,2,121,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1332,2,133,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1332,2,137,0,0,"IF", 1, 2500, 0);
	trans[6][14]	= settr(1197,2,164,249,249,"((transition_id==-(1)))", 1, 2500, 0);
	trans[6][15]	= settr(1198,2,164,1,0,"goto :b21", 1, 2500, 0);
	trans[6][150]	= settr(1333,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[6][16]	= settr(1199,2,151,250,250,"((transition_id==0))", 1, 2500, 0); /* m: 17 -> 151,0 */
	reached6[17] = 1;
	trans[6][17]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][18]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[6][19]	= settr(1202,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][20]	= settr(1203,2,22,251,251,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 6][22] = settr(1205,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1205,0,21,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][21]	= settr(1204,2,60,1,0,"(1)", 1, 2500, 0);
	T = trans[6][60] = settr(1243,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1243,2,23,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1243,2,39,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1243,2,57,0,0,"IF", 1, 2500, 0);
	trans[6][23]	= settr(1206,2,27,252,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 6][27] = settr(1210,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1210,0,24,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][24]	= settr(1207,2,26,253,253,"global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 6][26] = settr(1209,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1209,0,25,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][25]	= settr(1208,2,31,254,0,"assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 6][31] = settr(1214,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1214,0,28,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][28]	= settr(1211,2,30,255,255,"global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 6][30] = settr(1213,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1213,0,29,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][29]	= settr(1212,2,33,256,0,"assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))", 1, 2500, 0);
	T = trans[ 6][33] = settr(1216,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1216,0,32,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][32]	= settr(1215,2,35,257,257,"Actuator1_actuatorTc_channel!0", 1, 2500, 0);
	T = trans[ 6][35] = settr(1218,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1218,0,34,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][34]	= settr(1217,2,151,258,258,"Actuator2_actuatorTc_channel!0", 1, 2500, 0); /* m: 36 -> 151,0 */
	reached6[36] = 1;
	trans[6][36]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][37]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[6][38]	= settr(1221,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][61]	= settr(1244,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[6][39]	= settr(1222,2,52,259,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 6][52] = settr(1235,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1235,0,43,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 6][43] = settr(1226,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1226,0,40,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][40]	= settr(1223,2,42,260,260,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 6][42] = settr(1225,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1225,0,41,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][41]	= settr(1224,2,47,261,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 6][47] = settr(1230,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1230,0,44,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][44]	= settr(1227,2,46,262,262,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 6][46] = settr(1229,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1229,0,45,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][45]	= settr(1228,2,51,263,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 6][51] = settr(1234,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1234,0,48,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][48]	= settr(1231,2,50,264,264,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 6][50] = settr(1233,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1233,0,49,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][49]	= settr(1232,2,54,265,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 6][54] = settr(1237,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1237,0,53,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][53]	= settr(1236,2,151,266,266,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 55 -> 151,0 */
	reached6[55] = 1;
	trans[6][55]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][56]	= settr(1239,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][57]	= settr(1240,2,58,2,0,"else", 1, 2500, 0);
	trans[6][58]	= settr(1241,2,151,267,267,"transition_id = -(1)", 1, 2500, 0);
	trans[6][59]	= settr(1242,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][62]	= settr(1245,2,64,268,268,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 6][64] = settr(1247,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1247,0,63,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][63]	= settr(1246,2,94,1,0,"(1)", 1, 2500, 0);
	T = trans[6][94] = settr(1277,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1277,2,65,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1277,2,73,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1277,2,91,0,0,"IF", 1, 2500, 0);
	trans[6][65]	= settr(1248,2,67,269,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 6][67] = settr(1250,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1250,0,66,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][66]	= settr(1249,2,69,270,270,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[ 6][69] = settr(1252,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1252,0,68,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][68]	= settr(1251,2,151,271,271,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 70 -> 151,0 */
	reached6[70] = 1;
	trans[6][70]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][71]	= settr(0,0,0,0,0,"global_state.modemanager.state = 2",0,0,0);
	trans[6][72]	= settr(1255,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][95]	= settr(1278,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[6][73]	= settr(1256,2,86,272,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 6][86] = settr(1269,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1269,0,77,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 6][77] = settr(1260,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1260,0,74,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][74]	= settr(1257,2,76,273,273,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 6][76] = settr(1259,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1259,0,75,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][75]	= settr(1258,2,81,274,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 6][81] = settr(1264,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1264,0,78,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][78]	= settr(1261,2,80,275,275,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 6][80] = settr(1263,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1263,0,79,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][79]	= settr(1262,2,85,276,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 6][85] = settr(1268,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1268,0,82,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][82]	= settr(1265,2,84,277,277,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 6][84] = settr(1267,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1267,0,83,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][83]	= settr(1266,2,88,278,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 6][88] = settr(1271,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1271,0,87,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][87]	= settr(1270,2,151,279,279,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 89 -> 151,0 */
	reached6[89] = 1;
	trans[6][89]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][90]	= settr(1273,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][91]	= settr(1274,2,92,2,0,"else", 1, 2500, 0);
	trans[6][92]	= settr(1275,2,151,280,280,"transition_id = -(1)", 1, 2500, 0);
	trans[6][93]	= settr(1276,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][96]	= settr(1279,2,98,281,281,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 6][98] = settr(1281,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1281,0,97,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][97]	= settr(1280,2,105,1,0,"(1)", 1, 2500, 0);
	T = trans[6][105] = settr(1288,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1288,2,99,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1288,2,102,0,0,"IF", 1, 2500, 0);
	trans[6][99]	= settr(1282,2,101,282,0,"((global_state.modemanager.r<global_state.modemanager.threshold1))", 1, 2500, 0);
	T = trans[ 6][101] = settr(1284,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1284,0,100,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][100]	= settr(1283,2,113,283,283,"Actuator1_actuatorTc_channel!1", 1, 2500, 0);
	trans[6][106]	= settr(1289,2,113,1,0,".(goto)", 1, 2500, 0);
	trans[6][102]	= settr(1285,2,104,2,0,"else", 1, 2500, 0);
	T = trans[ 6][104] = settr(1287,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1287,0,103,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][103]	= settr(1286,2,113,284,284,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[6][113] = settr(1296,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1296,2,107,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1296,2,110,0,0,"IF", 1, 2500, 0);
	trans[6][107]	= settr(1290,2,109,285,0,"((global_state.modemanager.r>global_state.modemanager.threshold2))", 1, 2500, 0);
	T = trans[ 6][109] = settr(1292,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1292,0,108,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][108]	= settr(1291,2,151,286,286,"Actuator2_actuatorTc_channel!1", 1, 2500, 0); /* m: 115 -> 151,0 */
	reached6[115] = 1;
	trans[6][114]	= settr(1297,2,115,1,0,".(goto)", 1, 2500, 0); /* m: 115 -> 0,151 */
	reached6[115] = 1;
	trans[6][110]	= settr(1293,2,112,2,0,"else", 1, 2500, 0);
	T = trans[ 6][112] = settr(1295,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1295,0,111,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][111]	= settr(1294,2,151,287,287,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 115 -> 151,0 */
	reached6[115] = 1;
	trans[6][115]	= settr(1298,2,151,288,288,"transition_id = -(1)", 1, 2500, 0);
	trans[6][116]	= settr(1299,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][117]	= settr(1300,2,119,289,289,"((transition_id==4))", 1, 2500, 0);
	T = trans[ 6][119] = settr(1302,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1302,0,118,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][118]	= settr(1301,2,131,1,0,"(1)", 1, 2500, 0);
	trans[6][120]	= settr(1303,2,131,1,0,"goto startcheck", 1, 2500, 0);
	trans[6][121]	= settr(1304,2,123,290,290,"((transition_id==5))", 1, 2500, 0);
	T = trans[ 6][123] = settr(1306,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1306,0,122,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][122]	= settr(1305,2,131,1,0,"(1)", 1, 2500, 0);
	T = trans[6][131] = settr(1314,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1314,2,124,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1314,2,127,0,0,"IF", 1, 2500, 0);
	trans[6][124]	= settr(1307,2,151,291,291,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))", 1, 2500, 0); /* m: 125 -> 151,0 */
	reached6[125] = 1;
	trans[6][125]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][126]	= settr(1309,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][132]	= settr(1315,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[6][127]	= settr(1310,2,151,292,292,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))", 1, 2500, 0); /* m: 128 -> 151,0 */
	reached6[128] = 1;
	trans[6][128]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][129]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[6][130]	= settr(1313,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][133]	= settr(1316,2,135,293,293,"((transition_id==6))", 1, 2500, 0);
	T = trans[ 6][135] = settr(1318,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1318,0,134,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][134]	= settr(1317,2,147,1,0,"(1)", 1, 2500, 0);
	trans[6][136]	= settr(1319,2,147,1,0,"goto stopcheck", 1, 2500, 0);
	trans[6][137]	= settr(1320,2,139,294,294,"((transition_id==7))", 1, 2500, 0);
	T = trans[ 6][139] = settr(1322,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1322,0,138,0,0,"sub-sequence", 1, 2500, 0);
	trans[6][138]	= settr(1321,2,147,1,0,"(1)", 1, 2500, 0);
	T = trans[6][147] = settr(1330,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1330,2,140,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1330,2,143,0,0,"IF", 1, 2500, 0);
	trans[6][140]	= settr(1323,2,151,295,295,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))", 1, 2500, 0); /* m: 141 -> 151,0 */
	reached6[141] = 1;
	trans[6][141]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][142]	= settr(1325,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][148]	= settr(1331,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[6][143]	= settr(1326,2,151,296,296,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))", 1, 2500, 0); /* m: 144 -> 151,0 */
	reached6[144] = 1;
	trans[6][144]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[6][145]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[6][146]	= settr(1329,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[6][151]	= settr(1334,2,152,1,0,"(1)", 1, 2500, 0);
	trans[6][154]	= settr(1337,2,164,1,0,"break", 1, 2500, 0);
	trans[6][159]	= settr(1342,2,164,1,0,".(goto)", 1, 2500, 0);
	trans[6][156]	= settr(1339,2,157,2,0,"else", 1, 2500, 0);
	trans[6][157]	= settr(1340,2,164,1,0,"(1)", 1, 2500, 0);
	trans[6][161]	= settr(1344,2,164,1,0,"goto Modemanager_reading_loop", 1, 2500, 0);
	trans[6][165]	= settr(1348,2,166,1,0,".(goto)", 1, 2500, 0);
	trans[6][162]	= settr(1345,2,163,297,0,"(empty(Modemanager_reading_channel))", 1, 2500, 0);
	trans[6][163]	= settr(1346,2,166,298,298,"(1)", 1, 2500, 0); /* m: 165 -> 166,0 */
	reached6[165] = 1;
	trans[6][166]	= settr(1349,0,168,299,299,"Modemanager_lock!1", 1, 2500, 0);
	trans[6][170]	= settr(1353,0,171,1,0,"break", 0, 2, 0);
	trans[6][171]	= settr(1354,0,0,300,300,"-end-", 0, 3500, 0);

	/* proctype 5: Modemanager_actuatorTm2 */

	trans[5] = (Trans **) emalloc(321*sizeof(Trans *));

	trans[5][1]	= settr(864,0,317,301,0,"(inited)", 1, 2, 0);
	trans[5][318]	= settr(1181,0,317,1,0,".(goto)", 0, 2, 0);
	T = trans[5][317] = settr(1180,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(1180,0,316,0,0,"DO", 0, 2, 0);
	T = trans[ 5][316] = settr(1179,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(1179,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[5][2]	= settr(865,2,3,302,0,"(nempty(Modemanager_actuatorTm2_channel))", 1, 2500, 0);
	trans[5][3]	= settr(866,2,313,303,303,"Modemanager_lock?_", 1, 2500, 0);
	T = trans[5][313] = settr(1176,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1176,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1176,2,311,0,0,"IF", 1, 2500, 0);
	trans[5][4]	= settr(867,2,5,304,0,"(nempty(Modemanager_actuatorTm2_channel))", 1, 2500, 0);
	trans[5][5]	= settr(868,2,309,305,305,"Modemanager_actuatorTm2_channel?Modemanager_actuatortm2_signal_parameter", 1, 2500, 0); /* m: 6 -> 309,0 */
	reached5[6] = 1;
	trans[5][6]	= settr(0,0,0,0,0,"Modemanager_actuatortm2_channel_used = 1",0,0,0);
	T = trans[ 5][309] = settr(1172,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1172,0,307,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[5][307] = settr(1170,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1170,2,7,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1170,2,156,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1170,2,305,0,0,"IF", 1, 2500, 0);
	trans[5][7]	= settr(870,2,11,306,0,"((global_state.modemanager.state==0))", 1, 2500, 0);
	T = trans[ 5][11] = settr(874,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(874,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][8]	= settr(871,2,10,307,307,"global_state.modemanager.status2 = Modemanager_actuatortm2_signal_parameter", 1, 2500, 0);
	T = trans[ 5][10] = settr(873,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(873,0,9,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][9]	= settr(872,2,155,308,0,"assert((((global_state.modemanager.status2==0)||(global_state.modemanager.status2==1))||(global_state.modemanager.status2==2)))", 1, 2500, 0);
	T = trans[ 5][155] = settr(1018,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1018,0,12,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][12]	= settr(875,2,152,309,309,"transition_id = 0", 1, 2500, 0); /* m: 13 -> 0,152 */
	reached5[13] = 1;
	trans[5][13]	= settr(0,0,0,0,0,"transition_id = 4",0,0,0);
	trans[5][153]	= settr(1016,2,152,1,0,".(goto)", 1, 2500, 0);
	T = trans[5][152] = settr(1015,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(1015,2,149,0,0,"DO", 1, 2500, 0);
	T = trans[5][149] = settr(1012,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1012,2,14,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1012,2,16,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1012,2,20,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1012,2,62,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1012,2,96,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1012,2,117,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1012,2,121,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1012,2,133,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1012,2,137,0,0,"IF", 1, 2500, 0);
	trans[5][14]	= settr(877,2,313,310,310,"((transition_id==-(1)))", 1, 2500, 0);
	trans[5][15]	= settr(878,2,313,1,0,"goto :b18", 1, 2500, 0);
	trans[5][150]	= settr(1013,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[5][16]	= settr(879,2,151,311,311,"((transition_id==0))", 1, 2500, 0); /* m: 17 -> 151,0 */
	reached5[17] = 1;
	trans[5][17]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][18]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[5][19]	= settr(882,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][20]	= settr(883,2,22,312,312,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 5][22] = settr(885,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(885,0,21,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][21]	= settr(884,2,60,1,0,"(1)", 1, 2500, 0);
	T = trans[5][60] = settr(923,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(923,2,23,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(923,2,39,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(923,2,57,0,0,"IF", 1, 2500, 0);
	trans[5][23]	= settr(886,2,27,313,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 5][27] = settr(890,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(890,0,24,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][24]	= settr(887,2,26,314,314,"global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 5][26] = settr(889,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(889,0,25,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][25]	= settr(888,2,31,315,0,"assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 5][31] = settr(894,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(894,0,28,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][28]	= settr(891,2,30,316,316,"global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 5][30] = settr(893,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(893,0,29,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][29]	= settr(892,2,33,317,0,"assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))", 1, 2500, 0);
	T = trans[ 5][33] = settr(896,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(896,0,32,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][32]	= settr(895,2,35,318,318,"Actuator1_actuatorTc_channel!0", 1, 2500, 0);
	T = trans[ 5][35] = settr(898,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(898,0,34,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][34]	= settr(897,2,151,319,319,"Actuator2_actuatorTc_channel!0", 1, 2500, 0); /* m: 36 -> 151,0 */
	reached5[36] = 1;
	trans[5][36]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][37]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[5][38]	= settr(901,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][61]	= settr(924,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[5][39]	= settr(902,2,52,320,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 5][52] = settr(915,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(915,0,43,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 5][43] = settr(906,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(906,0,40,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][40]	= settr(903,2,42,321,321,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 5][42] = settr(905,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(905,0,41,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][41]	= settr(904,2,47,322,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 5][47] = settr(910,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(910,0,44,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][44]	= settr(907,2,46,323,323,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 5][46] = settr(909,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(909,0,45,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][45]	= settr(908,2,51,324,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 5][51] = settr(914,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(914,0,48,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][48]	= settr(911,2,50,325,325,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 5][50] = settr(913,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(913,0,49,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][49]	= settr(912,2,54,326,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 5][54] = settr(917,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(917,0,53,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][53]	= settr(916,2,151,327,327,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 55 -> 151,0 */
	reached5[55] = 1;
	trans[5][55]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][56]	= settr(919,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][57]	= settr(920,2,58,2,0,"else", 1, 2500, 0);
	trans[5][58]	= settr(921,2,151,328,328,"transition_id = -(1)", 1, 2500, 0);
	trans[5][59]	= settr(922,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][62]	= settr(925,2,64,329,329,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 5][64] = settr(927,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(927,0,63,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][63]	= settr(926,2,94,1,0,"(1)", 1, 2500, 0);
	T = trans[5][94] = settr(957,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(957,2,65,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(957,2,73,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(957,2,91,0,0,"IF", 1, 2500, 0);
	trans[5][65]	= settr(928,2,67,330,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 5][67] = settr(930,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(930,0,66,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][66]	= settr(929,2,69,331,331,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[ 5][69] = settr(932,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(932,0,68,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][68]	= settr(931,2,151,332,332,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 70 -> 151,0 */
	reached5[70] = 1;
	trans[5][70]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][71]	= settr(0,0,0,0,0,"global_state.modemanager.state = 2",0,0,0);
	trans[5][72]	= settr(935,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][95]	= settr(958,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[5][73]	= settr(936,2,86,333,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 5][86] = settr(949,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(949,0,77,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 5][77] = settr(940,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(940,0,74,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][74]	= settr(937,2,76,334,334,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 5][76] = settr(939,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(939,0,75,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][75]	= settr(938,2,81,335,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 5][81] = settr(944,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(944,0,78,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][78]	= settr(941,2,80,336,336,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 5][80] = settr(943,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(943,0,79,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][79]	= settr(942,2,85,337,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 5][85] = settr(948,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(948,0,82,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][82]	= settr(945,2,84,338,338,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 5][84] = settr(947,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(947,0,83,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][83]	= settr(946,2,88,339,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 5][88] = settr(951,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(951,0,87,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][87]	= settr(950,2,151,340,340,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 89 -> 151,0 */
	reached5[89] = 1;
	trans[5][89]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][90]	= settr(953,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][91]	= settr(954,2,92,2,0,"else", 1, 2500, 0);
	trans[5][92]	= settr(955,2,151,341,341,"transition_id = -(1)", 1, 2500, 0);
	trans[5][93]	= settr(956,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][96]	= settr(959,2,98,342,342,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 5][98] = settr(961,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(961,0,97,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][97]	= settr(960,2,105,1,0,"(1)", 1, 2500, 0);
	T = trans[5][105] = settr(968,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(968,2,99,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(968,2,102,0,0,"IF", 1, 2500, 0);
	trans[5][99]	= settr(962,2,101,343,0,"((global_state.modemanager.r<global_state.modemanager.threshold1))", 1, 2500, 0);
	T = trans[ 5][101] = settr(964,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(964,0,100,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][100]	= settr(963,2,113,344,344,"Actuator1_actuatorTc_channel!1", 1, 2500, 0);
	trans[5][106]	= settr(969,2,113,1,0,".(goto)", 1, 2500, 0);
	trans[5][102]	= settr(965,2,104,2,0,"else", 1, 2500, 0);
	T = trans[ 5][104] = settr(967,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(967,0,103,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][103]	= settr(966,2,113,345,345,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[5][113] = settr(976,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(976,2,107,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(976,2,110,0,0,"IF", 1, 2500, 0);
	trans[5][107]	= settr(970,2,109,346,0,"((global_state.modemanager.r>global_state.modemanager.threshold2))", 1, 2500, 0);
	T = trans[ 5][109] = settr(972,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(972,0,108,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][108]	= settr(971,2,151,347,347,"Actuator2_actuatorTc_channel!1", 1, 2500, 0); /* m: 115 -> 151,0 */
	reached5[115] = 1;
	trans[5][114]	= settr(977,2,115,1,0,".(goto)", 1, 2500, 0); /* m: 115 -> 0,151 */
	reached5[115] = 1;
	trans[5][110]	= settr(973,2,112,2,0,"else", 1, 2500, 0);
	T = trans[ 5][112] = settr(975,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(975,0,111,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][111]	= settr(974,2,151,348,348,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 115 -> 151,0 */
	reached5[115] = 1;
	trans[5][115]	= settr(978,2,151,349,349,"transition_id = -(1)", 1, 2500, 0);
	trans[5][116]	= settr(979,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][117]	= settr(980,2,119,350,350,"((transition_id==4))", 1, 2500, 0);
	T = trans[ 5][119] = settr(982,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(982,0,118,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][118]	= settr(981,2,131,1,0,"(1)", 1, 2500, 0);
	trans[5][120]	= settr(983,2,131,1,0,"goto startcheck", 1, 2500, 0);
	trans[5][121]	= settr(984,2,123,351,351,"((transition_id==5))", 1, 2500, 0);
	T = trans[ 5][123] = settr(986,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(986,0,122,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][122]	= settr(985,2,131,1,0,"(1)", 1, 2500, 0);
	T = trans[5][131] = settr(994,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(994,2,124,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(994,2,127,0,0,"IF", 1, 2500, 0);
	trans[5][124]	= settr(987,2,151,352,352,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))", 1, 2500, 0); /* m: 125 -> 151,0 */
	reached5[125] = 1;
	trans[5][125]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][126]	= settr(989,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][132]	= settr(995,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[5][127]	= settr(990,2,151,353,353,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))", 1, 2500, 0); /* m: 128 -> 151,0 */
	reached5[128] = 1;
	trans[5][128]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][129]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[5][130]	= settr(993,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][133]	= settr(996,2,135,354,354,"((transition_id==6))", 1, 2500, 0);
	T = trans[ 5][135] = settr(998,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(998,0,134,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][134]	= settr(997,2,147,1,0,"(1)", 1, 2500, 0);
	trans[5][136]	= settr(999,2,147,1,0,"goto stopcheck", 1, 2500, 0);
	trans[5][137]	= settr(1000,2,139,355,355,"((transition_id==7))", 1, 2500, 0);
	T = trans[ 5][139] = settr(1002,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1002,0,138,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][138]	= settr(1001,2,147,1,0,"(1)", 1, 2500, 0);
	T = trans[5][147] = settr(1010,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1010,2,140,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1010,2,143,0,0,"IF", 1, 2500, 0);
	trans[5][140]	= settr(1003,2,151,356,356,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))", 1, 2500, 0); /* m: 141 -> 151,0 */
	reached5[141] = 1;
	trans[5][141]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][142]	= settr(1005,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][148]	= settr(1011,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[5][143]	= settr(1006,2,151,357,357,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))", 1, 2500, 0); /* m: 144 -> 151,0 */
	reached5[144] = 1;
	trans[5][144]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][145]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[5][146]	= settr(1009,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][151]	= settr(1014,2,152,1,0,"(1)", 1, 2500, 0);
	trans[5][154]	= settr(1017,2,313,1,0,"break", 1, 2500, 0);
	trans[5][308]	= settr(1171,2,313,1,0,".(goto)", 1, 2500, 0);
	trans[5][156]	= settr(1019,2,160,358,0,"((global_state.modemanager.state==2))", 1, 2500, 0);
	T = trans[ 5][160] = settr(1023,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1023,0,157,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][157]	= settr(1020,2,159,359,359,"global_state.modemanager.status2 = Modemanager_actuatortm2_signal_parameter", 1, 2500, 0);
	T = trans[ 5][159] = settr(1022,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1022,0,158,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][158]	= settr(1021,2,304,360,0,"assert((((global_state.modemanager.status2==0)||(global_state.modemanager.status2==1))||(global_state.modemanager.status2==2)))", 1, 2500, 0);
	T = trans[ 5][304] = settr(1167,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1167,0,161,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][161]	= settr(1024,2,301,361,361,"transition_id = 0", 1, 2500, 0); /* m: 162 -> 0,301 */
	reached5[162] = 1;
	trans[5][162]	= settr(0,0,0,0,0,"transition_id = 6",0,0,0);
	trans[5][302]	= settr(1165,2,301,1,0,".(goto)", 1, 2500, 0);
	T = trans[5][301] = settr(1164,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(1164,2,298,0,0,"DO", 1, 2500, 0);
	T = trans[5][298] = settr(1161,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1161,2,163,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1161,2,165,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1161,2,169,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1161,2,211,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1161,2,245,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1161,2,266,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1161,2,270,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1161,2,282,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1161,2,286,0,0,"IF", 1, 2500, 0);
	trans[5][163]	= settr(1026,2,313,362,362,"((transition_id==-(1)))", 1, 2500, 0);
	trans[5][164]	= settr(1027,2,313,1,0,"goto :b19", 1, 2500, 0);
	trans[5][299]	= settr(1162,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[5][165]	= settr(1028,2,300,363,363,"((transition_id==0))", 1, 2500, 0); /* m: 166 -> 300,0 */
	reached5[166] = 1;
	trans[5][166]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][167]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[5][168]	= settr(1031,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][169]	= settr(1032,2,171,364,364,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 5][171] = settr(1034,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1034,0,170,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][170]	= settr(1033,2,209,1,0,"(1)", 1, 2500, 0);
	T = trans[5][209] = settr(1072,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1072,2,172,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1072,2,188,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1072,2,206,0,0,"IF", 1, 2500, 0);
	trans[5][172]	= settr(1035,2,176,365,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 5][176] = settr(1039,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1039,0,173,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][173]	= settr(1036,2,175,366,366,"global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 5][175] = settr(1038,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1038,0,174,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][174]	= settr(1037,2,180,367,0,"assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 5][180] = settr(1043,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1043,0,177,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][177]	= settr(1040,2,179,368,368,"global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 5][179] = settr(1042,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1042,0,178,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][178]	= settr(1041,2,182,369,0,"assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))", 1, 2500, 0);
	T = trans[ 5][182] = settr(1045,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1045,0,181,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][181]	= settr(1044,2,184,370,370,"Actuator1_actuatorTc_channel!0", 1, 2500, 0);
	T = trans[ 5][184] = settr(1047,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1047,0,183,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][183]	= settr(1046,2,300,371,371,"Actuator2_actuatorTc_channel!0", 1, 2500, 0); /* m: 185 -> 300,0 */
	reached5[185] = 1;
	trans[5][185]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][186]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[5][187]	= settr(1050,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][210]	= settr(1073,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[5][188]	= settr(1051,2,201,372,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 5][201] = settr(1064,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1064,0,192,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 5][192] = settr(1055,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1055,0,189,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][189]	= settr(1052,2,191,373,373,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 5][191] = settr(1054,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1054,0,190,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][190]	= settr(1053,2,196,374,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 5][196] = settr(1059,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1059,0,193,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][193]	= settr(1056,2,195,375,375,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 5][195] = settr(1058,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1058,0,194,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][194]	= settr(1057,2,200,376,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 5][200] = settr(1063,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1063,0,197,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][197]	= settr(1060,2,199,377,377,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 5][199] = settr(1062,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1062,0,198,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][198]	= settr(1061,2,203,378,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 5][203] = settr(1066,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1066,0,202,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][202]	= settr(1065,2,300,379,379,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 204 -> 300,0 */
	reached5[204] = 1;
	trans[5][204]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][205]	= settr(1068,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][206]	= settr(1069,2,207,2,0,"else", 1, 2500, 0);
	trans[5][207]	= settr(1070,2,300,380,380,"transition_id = -(1)", 1, 2500, 0);
	trans[5][208]	= settr(1071,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][211]	= settr(1074,2,213,381,381,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 5][213] = settr(1076,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1076,0,212,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][212]	= settr(1075,2,243,1,0,"(1)", 1, 2500, 0);
	T = trans[5][243] = settr(1106,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1106,2,214,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1106,2,222,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1106,2,240,0,0,"IF", 1, 2500, 0);
	trans[5][214]	= settr(1077,2,216,382,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 5][216] = settr(1079,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1079,0,215,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][215]	= settr(1078,2,218,383,383,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[ 5][218] = settr(1081,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1081,0,217,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][217]	= settr(1080,2,300,384,384,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 219 -> 300,0 */
	reached5[219] = 1;
	trans[5][219]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][220]	= settr(0,0,0,0,0,"global_state.modemanager.state = 2",0,0,0);
	trans[5][221]	= settr(1084,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][244]	= settr(1107,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[5][222]	= settr(1085,2,235,385,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 5][235] = settr(1098,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1098,0,226,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 5][226] = settr(1089,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1089,0,223,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][223]	= settr(1086,2,225,386,386,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 5][225] = settr(1088,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1088,0,224,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][224]	= settr(1087,2,230,387,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 5][230] = settr(1093,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1093,0,227,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][227]	= settr(1090,2,229,388,388,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 5][229] = settr(1092,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1092,0,228,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][228]	= settr(1091,2,234,389,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 5][234] = settr(1097,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1097,0,231,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][231]	= settr(1094,2,233,390,390,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 5][233] = settr(1096,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1096,0,232,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][232]	= settr(1095,2,237,391,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 5][237] = settr(1100,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1100,0,236,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][236]	= settr(1099,2,300,392,392,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 238 -> 300,0 */
	reached5[238] = 1;
	trans[5][238]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][239]	= settr(1102,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][240]	= settr(1103,2,241,2,0,"else", 1, 2500, 0);
	trans[5][241]	= settr(1104,2,300,393,393,"transition_id = -(1)", 1, 2500, 0);
	trans[5][242]	= settr(1105,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][245]	= settr(1108,2,247,394,394,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 5][247] = settr(1110,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1110,0,246,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][246]	= settr(1109,2,254,1,0,"(1)", 1, 2500, 0);
	T = trans[5][254] = settr(1117,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1117,2,248,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1117,2,251,0,0,"IF", 1, 2500, 0);
	trans[5][248]	= settr(1111,2,250,395,0,"((global_state.modemanager.r<global_state.modemanager.threshold1))", 1, 2500, 0);
	T = trans[ 5][250] = settr(1113,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1113,0,249,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][249]	= settr(1112,2,262,396,396,"Actuator1_actuatorTc_channel!1", 1, 2500, 0);
	trans[5][255]	= settr(1118,2,262,1,0,".(goto)", 1, 2500, 0);
	trans[5][251]	= settr(1114,2,253,2,0,"else", 1, 2500, 0);
	T = trans[ 5][253] = settr(1116,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1116,0,252,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][252]	= settr(1115,2,262,397,397,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[5][262] = settr(1125,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1125,2,256,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1125,2,259,0,0,"IF", 1, 2500, 0);
	trans[5][256]	= settr(1119,2,258,398,0,"((global_state.modemanager.r>global_state.modemanager.threshold2))", 1, 2500, 0);
	T = trans[ 5][258] = settr(1121,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1121,0,257,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][257]	= settr(1120,2,300,399,399,"Actuator2_actuatorTc_channel!1", 1, 2500, 0); /* m: 264 -> 300,0 */
	reached5[264] = 1;
	trans[5][263]	= settr(1126,2,264,1,0,".(goto)", 1, 2500, 0); /* m: 264 -> 0,300 */
	reached5[264] = 1;
	trans[5][259]	= settr(1122,2,261,2,0,"else", 1, 2500, 0);
	T = trans[ 5][261] = settr(1124,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1124,0,260,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][260]	= settr(1123,2,300,400,400,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 264 -> 300,0 */
	reached5[264] = 1;
	trans[5][264]	= settr(1127,2,300,401,401,"transition_id = -(1)", 1, 2500, 0);
	trans[5][265]	= settr(1128,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][266]	= settr(1129,2,268,402,402,"((transition_id==4))", 1, 2500, 0);
	T = trans[ 5][268] = settr(1131,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1131,0,267,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][267]	= settr(1130,2,280,1,0,"(1)", 1, 2500, 0);
	trans[5][269]	= settr(1132,2,280,1,0,"goto startcheck", 1, 2500, 0);
	trans[5][270]	= settr(1133,2,272,403,403,"((transition_id==5))", 1, 2500, 0);
	T = trans[ 5][272] = settr(1135,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1135,0,271,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][271]	= settr(1134,2,280,1,0,"(1)", 1, 2500, 0);
	T = trans[5][280] = settr(1143,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1143,2,273,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1143,2,276,0,0,"IF", 1, 2500, 0);
	trans[5][273]	= settr(1136,2,300,404,404,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))", 1, 2500, 0); /* m: 274 -> 300,0 */
	reached5[274] = 1;
	trans[5][274]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][275]	= settr(1138,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][281]	= settr(1144,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[5][276]	= settr(1139,2,300,405,405,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))", 1, 2500, 0); /* m: 277 -> 300,0 */
	reached5[277] = 1;
	trans[5][277]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][278]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[5][279]	= settr(1142,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][282]	= settr(1145,2,284,406,406,"((transition_id==6))", 1, 2500, 0);
	T = trans[ 5][284] = settr(1147,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1147,0,283,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][283]	= settr(1146,2,296,1,0,"(1)", 1, 2500, 0);
	trans[5][285]	= settr(1148,2,296,1,0,"goto stopcheck", 1, 2500, 0);
	trans[5][286]	= settr(1149,2,288,407,407,"((transition_id==7))", 1, 2500, 0);
	T = trans[ 5][288] = settr(1151,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(1151,0,287,0,0,"sub-sequence", 1, 2500, 0);
	trans[5][287]	= settr(1150,2,296,1,0,"(1)", 1, 2500, 0);
	T = trans[5][296] = settr(1159,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(1159,2,289,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(1159,2,292,0,0,"IF", 1, 2500, 0);
	trans[5][289]	= settr(1152,2,300,408,408,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))", 1, 2500, 0); /* m: 290 -> 300,0 */
	reached5[290] = 1;
	trans[5][290]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][291]	= settr(1154,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][297]	= settr(1160,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[5][292]	= settr(1155,2,300,409,409,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))", 1, 2500, 0); /* m: 293 -> 300,0 */
	reached5[293] = 1;
	trans[5][293]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[5][294]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[5][295]	= settr(1158,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[5][300]	= settr(1163,2,301,1,0,"(1)", 1, 2500, 0);
	trans[5][303]	= settr(1166,2,313,1,0,"break", 1, 2500, 0);
	trans[5][305]	= settr(1168,2,306,2,0,"else", 1, 2500, 0);
	trans[5][306]	= settr(1169,2,313,1,0,"(1)", 1, 2500, 0);
	trans[5][310]	= settr(1173,2,313,1,0,"goto Modemanager_actuatortm2_loop", 1, 2500, 0);
	trans[5][314]	= settr(1177,2,315,1,0,".(goto)", 1, 2500, 0);
	trans[5][311]	= settr(1174,2,312,410,0,"(empty(Modemanager_actuatorTm2_channel))", 1, 2500, 0);
	trans[5][312]	= settr(1175,2,315,411,411,"(1)", 1, 2500, 0); /* m: 314 -> 315,0 */
	reached5[314] = 1;
	trans[5][315]	= settr(1178,0,317,412,412,"Modemanager_lock!1", 1, 2500, 0);
	trans[5][319]	= settr(1182,0,320,1,0,"break", 0, 2, 0);
	trans[5][320]	= settr(1183,0,0,413,413,"-end-", 0, 3500, 0);

	/* proctype 4: Modemanager_actuatorTm1 */

	trans[4] = (Trans **) emalloc(321*sizeof(Trans *));

	trans[4][1]	= settr(544,0,317,414,0,"(inited)", 1, 2, 0);
	trans[4][318]	= settr(861,0,317,1,0,".(goto)", 0, 2, 0);
	T = trans[4][317] = settr(860,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(860,0,316,0,0,"DO", 0, 2, 0);
	T = trans[ 4][316] = settr(859,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(859,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[4][2]	= settr(545,2,3,415,0,"(nempty(Modemanager_actuatorTm1_channel))", 1, 2500, 0);
	trans[4][3]	= settr(546,2,313,416,416,"Modemanager_lock?_", 1, 2500, 0);
	T = trans[4][313] = settr(856,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(856,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(856,2,311,0,0,"IF", 1, 2500, 0);
	trans[4][4]	= settr(547,2,5,417,0,"(nempty(Modemanager_actuatorTm1_channel))", 1, 2500, 0);
	trans[4][5]	= settr(548,2,309,418,418,"Modemanager_actuatorTm1_channel?Modemanager_actuatortm1_signal_parameter", 1, 2500, 0); /* m: 6 -> 309,0 */
	reached4[6] = 1;
	trans[4][6]	= settr(0,0,0,0,0,"Modemanager_actuatortm1_channel_used = 1",0,0,0);
	T = trans[ 4][309] = settr(852,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(852,0,307,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[4][307] = settr(850,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(850,2,7,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(850,2,156,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(850,2,305,0,0,"IF", 1, 2500, 0);
	trans[4][7]	= settr(550,2,11,419,0,"((global_state.modemanager.state==0))", 1, 2500, 0);
	T = trans[ 4][11] = settr(554,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(554,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][8]	= settr(551,2,10,420,420,"global_state.modemanager.status1 = Modemanager_actuatortm1_signal_parameter", 1, 2500, 0);
	T = trans[ 4][10] = settr(553,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(553,0,9,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][9]	= settr(552,2,155,421,0,"assert((((global_state.modemanager.status1==0)||(global_state.modemanager.status1==1))||(global_state.modemanager.status1==2)))", 1, 2500, 0);
	T = trans[ 4][155] = settr(698,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(698,0,12,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][12]	= settr(555,2,152,422,422,"transition_id = 0", 1, 2500, 0); /* m: 13 -> 0,152 */
	reached4[13] = 1;
	trans[4][13]	= settr(0,0,0,0,0,"transition_id = 5",0,0,0);
	trans[4][153]	= settr(696,2,152,1,0,".(goto)", 1, 2500, 0);
	T = trans[4][152] = settr(695,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(695,2,149,0,0,"DO", 1, 2500, 0);
	T = trans[4][149] = settr(692,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(692,2,14,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(692,2,16,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(692,2,20,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(692,2,62,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(692,2,96,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(692,2,117,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(692,2,121,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(692,2,133,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(692,2,137,0,0,"IF", 1, 2500, 0);
	trans[4][14]	= settr(557,2,313,423,423,"((transition_id==-(1)))", 1, 2500, 0);
	trans[4][15]	= settr(558,2,313,1,0,"goto :b15", 1, 2500, 0);
	trans[4][150]	= settr(693,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[4][16]	= settr(559,2,151,424,424,"((transition_id==0))", 1, 2500, 0); /* m: 17 -> 151,0 */
	reached4[17] = 1;
	trans[4][17]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][18]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[4][19]	= settr(562,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][20]	= settr(563,2,22,425,425,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 4][22] = settr(565,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(565,0,21,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][21]	= settr(564,2,60,1,0,"(1)", 1, 2500, 0);
	T = trans[4][60] = settr(603,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(603,2,23,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(603,2,39,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(603,2,57,0,0,"IF", 1, 2500, 0);
	trans[4][23]	= settr(566,2,27,426,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 4][27] = settr(570,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(570,0,24,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][24]	= settr(567,2,26,427,427,"global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 4][26] = settr(569,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(569,0,25,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][25]	= settr(568,2,31,428,0,"assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 4][31] = settr(574,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(574,0,28,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][28]	= settr(571,2,30,429,429,"global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 4][30] = settr(573,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(573,0,29,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][29]	= settr(572,2,33,430,0,"assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))", 1, 2500, 0);
	T = trans[ 4][33] = settr(576,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(576,0,32,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][32]	= settr(575,2,35,431,431,"Actuator1_actuatorTc_channel!0", 1, 2500, 0);
	T = trans[ 4][35] = settr(578,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(578,0,34,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][34]	= settr(577,2,151,432,432,"Actuator2_actuatorTc_channel!0", 1, 2500, 0); /* m: 36 -> 151,0 */
	reached4[36] = 1;
	trans[4][36]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][37]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[4][38]	= settr(581,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][61]	= settr(604,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[4][39]	= settr(582,2,52,433,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 4][52] = settr(595,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(595,0,43,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 4][43] = settr(586,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(586,0,40,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][40]	= settr(583,2,42,434,434,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 4][42] = settr(585,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(585,0,41,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][41]	= settr(584,2,47,435,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 4][47] = settr(590,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(590,0,44,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][44]	= settr(587,2,46,436,436,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 4][46] = settr(589,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(589,0,45,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][45]	= settr(588,2,51,437,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 4][51] = settr(594,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(594,0,48,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][48]	= settr(591,2,50,438,438,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 4][50] = settr(593,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(593,0,49,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][49]	= settr(592,2,54,439,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 4][54] = settr(597,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(597,0,53,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][53]	= settr(596,2,151,440,440,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 55 -> 151,0 */
	reached4[55] = 1;
	trans[4][55]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][56]	= settr(599,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][57]	= settr(600,2,58,2,0,"else", 1, 2500, 0);
	trans[4][58]	= settr(601,2,151,441,441,"transition_id = -(1)", 1, 2500, 0);
	trans[4][59]	= settr(602,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][62]	= settr(605,2,64,442,442,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 4][64] = settr(607,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(607,0,63,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][63]	= settr(606,2,94,1,0,"(1)", 1, 2500, 0);
	T = trans[4][94] = settr(637,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(637,2,65,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(637,2,73,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(637,2,91,0,0,"IF", 1, 2500, 0);
	trans[4][65]	= settr(608,2,67,443,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 4][67] = settr(610,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(610,0,66,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][66]	= settr(609,2,69,444,444,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[ 4][69] = settr(612,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(612,0,68,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][68]	= settr(611,2,151,445,445,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 70 -> 151,0 */
	reached4[70] = 1;
	trans[4][70]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][71]	= settr(0,0,0,0,0,"global_state.modemanager.state = 2",0,0,0);
	trans[4][72]	= settr(615,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][95]	= settr(638,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[4][73]	= settr(616,2,86,446,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 4][86] = settr(629,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(629,0,77,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 4][77] = settr(620,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(620,0,74,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][74]	= settr(617,2,76,447,447,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 4][76] = settr(619,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(619,0,75,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][75]	= settr(618,2,81,448,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 4][81] = settr(624,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(624,0,78,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][78]	= settr(621,2,80,449,449,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 4][80] = settr(623,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(623,0,79,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][79]	= settr(622,2,85,450,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 4][85] = settr(628,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(628,0,82,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][82]	= settr(625,2,84,451,451,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 4][84] = settr(627,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(627,0,83,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][83]	= settr(626,2,88,452,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 4][88] = settr(631,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(631,0,87,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][87]	= settr(630,2,151,453,453,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 89 -> 151,0 */
	reached4[89] = 1;
	trans[4][89]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][90]	= settr(633,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][91]	= settr(634,2,92,2,0,"else", 1, 2500, 0);
	trans[4][92]	= settr(635,2,151,454,454,"transition_id = -(1)", 1, 2500, 0);
	trans[4][93]	= settr(636,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][96]	= settr(639,2,98,455,455,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 4][98] = settr(641,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(641,0,97,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][97]	= settr(640,2,105,1,0,"(1)", 1, 2500, 0);
	T = trans[4][105] = settr(648,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(648,2,99,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(648,2,102,0,0,"IF", 1, 2500, 0);
	trans[4][99]	= settr(642,2,101,456,0,"((global_state.modemanager.r<global_state.modemanager.threshold1))", 1, 2500, 0);
	T = trans[ 4][101] = settr(644,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(644,0,100,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][100]	= settr(643,2,113,457,457,"Actuator1_actuatorTc_channel!1", 1, 2500, 0);
	trans[4][106]	= settr(649,2,113,1,0,".(goto)", 1, 2500, 0);
	trans[4][102]	= settr(645,2,104,2,0,"else", 1, 2500, 0);
	T = trans[ 4][104] = settr(647,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(647,0,103,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][103]	= settr(646,2,113,458,458,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[4][113] = settr(656,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(656,2,107,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(656,2,110,0,0,"IF", 1, 2500, 0);
	trans[4][107]	= settr(650,2,109,459,0,"((global_state.modemanager.r>global_state.modemanager.threshold2))", 1, 2500, 0);
	T = trans[ 4][109] = settr(652,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(652,0,108,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][108]	= settr(651,2,151,460,460,"Actuator2_actuatorTc_channel!1", 1, 2500, 0); /* m: 115 -> 151,0 */
	reached4[115] = 1;
	trans[4][114]	= settr(657,2,115,1,0,".(goto)", 1, 2500, 0); /* m: 115 -> 0,151 */
	reached4[115] = 1;
	trans[4][110]	= settr(653,2,112,2,0,"else", 1, 2500, 0);
	T = trans[ 4][112] = settr(655,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(655,0,111,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][111]	= settr(654,2,151,461,461,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 115 -> 151,0 */
	reached4[115] = 1;
	trans[4][115]	= settr(658,2,151,462,462,"transition_id = -(1)", 1, 2500, 0);
	trans[4][116]	= settr(659,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][117]	= settr(660,2,119,463,463,"((transition_id==4))", 1, 2500, 0);
	T = trans[ 4][119] = settr(662,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(662,0,118,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][118]	= settr(661,2,131,1,0,"(1)", 1, 2500, 0);
	trans[4][120]	= settr(663,2,131,1,0,"goto startcheck", 1, 2500, 0);
	trans[4][121]	= settr(664,2,123,464,464,"((transition_id==5))", 1, 2500, 0);
	T = trans[ 4][123] = settr(666,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(666,0,122,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][122]	= settr(665,2,131,1,0,"(1)", 1, 2500, 0);
	T = trans[4][131] = settr(674,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(674,2,124,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(674,2,127,0,0,"IF", 1, 2500, 0);
	trans[4][124]	= settr(667,2,151,465,465,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))", 1, 2500, 0); /* m: 125 -> 151,0 */
	reached4[125] = 1;
	trans[4][125]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][126]	= settr(669,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][132]	= settr(675,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[4][127]	= settr(670,2,151,466,466,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))", 1, 2500, 0); /* m: 128 -> 151,0 */
	reached4[128] = 1;
	trans[4][128]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][129]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[4][130]	= settr(673,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][133]	= settr(676,2,135,467,467,"((transition_id==6))", 1, 2500, 0);
	T = trans[ 4][135] = settr(678,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(678,0,134,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][134]	= settr(677,2,147,1,0,"(1)", 1, 2500, 0);
	trans[4][136]	= settr(679,2,147,1,0,"goto stopcheck", 1, 2500, 0);
	trans[4][137]	= settr(680,2,139,468,468,"((transition_id==7))", 1, 2500, 0);
	T = trans[ 4][139] = settr(682,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(682,0,138,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][138]	= settr(681,2,147,1,0,"(1)", 1, 2500, 0);
	T = trans[4][147] = settr(690,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(690,2,140,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(690,2,143,0,0,"IF", 1, 2500, 0);
	trans[4][140]	= settr(683,2,151,469,469,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))", 1, 2500, 0); /* m: 141 -> 151,0 */
	reached4[141] = 1;
	trans[4][141]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][142]	= settr(685,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][148]	= settr(691,2,151,1,0,".(goto)", 1, 2500, 0);
	trans[4][143]	= settr(686,2,151,470,470,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))", 1, 2500, 0); /* m: 144 -> 151,0 */
	reached4[144] = 1;
	trans[4][144]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][145]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[4][146]	= settr(689,2,151,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][151]	= settr(694,2,152,1,0,"(1)", 1, 2500, 0);
	trans[4][154]	= settr(697,2,313,1,0,"break", 1, 2500, 0);
	trans[4][308]	= settr(851,2,313,1,0,".(goto)", 1, 2500, 0);
	trans[4][156]	= settr(699,2,160,471,0,"((global_state.modemanager.state==2))", 1, 2500, 0);
	T = trans[ 4][160] = settr(703,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(703,0,157,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][157]	= settr(700,2,159,472,472,"global_state.modemanager.status1 = Modemanager_actuatortm1_signal_parameter", 1, 2500, 0);
	T = trans[ 4][159] = settr(702,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(702,0,158,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][158]	= settr(701,2,304,473,0,"assert((((global_state.modemanager.status1==0)||(global_state.modemanager.status1==1))||(global_state.modemanager.status1==2)))", 1, 2500, 0);
	T = trans[ 4][304] = settr(847,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(847,0,161,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][161]	= settr(704,2,301,474,474,"transition_id = 0", 1, 2500, 0); /* m: 162 -> 0,301 */
	reached4[162] = 1;
	trans[4][162]	= settr(0,0,0,0,0,"transition_id = 7",0,0,0);
	trans[4][302]	= settr(845,2,301,1,0,".(goto)", 1, 2500, 0);
	T = trans[4][301] = settr(844,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(844,2,298,0,0,"DO", 1, 2500, 0);
	T = trans[4][298] = settr(841,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(841,2,163,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(841,2,165,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(841,2,169,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(841,2,211,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(841,2,245,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(841,2,266,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(841,2,270,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(841,2,282,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(841,2,286,0,0,"IF", 1, 2500, 0);
	trans[4][163]	= settr(706,2,313,475,475,"((transition_id==-(1)))", 1, 2500, 0);
	trans[4][164]	= settr(707,2,313,1,0,"goto :b16", 1, 2500, 0);
	trans[4][299]	= settr(842,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[4][165]	= settr(708,2,300,476,476,"((transition_id==0))", 1, 2500, 0); /* m: 166 -> 300,0 */
	reached4[166] = 1;
	trans[4][166]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][167]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[4][168]	= settr(711,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][169]	= settr(712,2,171,477,477,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 4][171] = settr(714,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(714,0,170,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][170]	= settr(713,2,209,1,0,"(1)", 1, 2500, 0);
	T = trans[4][209] = settr(752,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(752,2,172,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(752,2,188,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(752,2,206,0,0,"IF", 1, 2500, 0);
	trans[4][172]	= settr(715,2,176,478,0,"((global_state.modemanager.stc.selection==2))", 1, 2500, 0);
	T = trans[ 4][176] = settr(719,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(719,0,173,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][173]	= settr(716,2,175,479,479,"global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1", 1, 2500, 0);
	T = trans[ 4][175] = settr(718,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(718,0,174,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][174]	= settr(717,2,180,480,0,"assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))", 1, 2500, 0);
	T = trans[ 4][180] = settr(723,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(723,0,177,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][177]	= settr(720,2,179,481,481,"global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2", 1, 2500, 0);
	T = trans[ 4][179] = settr(722,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(722,0,178,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][178]	= settr(721,2,182,482,0,"assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))", 1, 2500, 0);
	T = trans[ 4][182] = settr(725,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(725,0,181,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][181]	= settr(724,2,184,483,483,"Actuator1_actuatorTc_channel!0", 1, 2500, 0);
	T = trans[ 4][184] = settr(727,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(727,0,183,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][183]	= settr(726,2,300,484,484,"Actuator2_actuatorTc_channel!0", 1, 2500, 0); /* m: 185 -> 300,0 */
	reached4[185] = 1;
	trans[4][185]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][186]	= settr(0,0,0,0,0,"global_state.modemanager.state = 0",0,0,0);
	trans[4][187]	= settr(730,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][210]	= settr(753,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[4][188]	= settr(731,2,201,485,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 4][201] = settr(744,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(744,0,192,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 4][192] = settr(735,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(735,0,189,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][189]	= settr(732,2,191,486,486,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 4][191] = settr(734,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(734,0,190,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][190]	= settr(733,2,196,487,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 4][196] = settr(739,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(739,0,193,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][193]	= settr(736,2,195,488,488,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 4][195] = settr(738,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(738,0,194,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][194]	= settr(737,2,200,489,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 4][200] = settr(743,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(743,0,197,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][197]	= settr(740,2,199,490,490,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 4][199] = settr(742,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(742,0,198,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][198]	= settr(741,2,203,491,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 4][203] = settr(746,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(746,0,202,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][202]	= settr(745,2,300,492,492,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 204 -> 300,0 */
	reached4[204] = 1;
	trans[4][204]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][205]	= settr(748,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][206]	= settr(749,2,207,2,0,"else", 1, 2500, 0);
	trans[4][207]	= settr(750,2,300,493,493,"transition_id = -(1)", 1, 2500, 0);
	trans[4][208]	= settr(751,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][211]	= settr(754,2,213,494,494,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 4][213] = settr(756,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(756,0,212,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][212]	= settr(755,2,243,1,0,"(1)", 1, 2500, 0);
	T = trans[4][243] = settr(786,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(786,2,214,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(786,2,222,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(786,2,240,0,0,"IF", 1, 2500, 0);
	trans[4][214]	= settr(757,2,216,495,0,"((global_state.modemanager.stc.selection==1))", 1, 2500, 0);
	T = trans[ 4][216] = settr(759,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(759,0,215,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][215]	= settr(758,2,218,496,496,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[ 4][218] = settr(761,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(761,0,217,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][217]	= settr(760,2,300,497,497,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 219 -> 300,0 */
	reached4[219] = 1;
	trans[4][219]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][220]	= settr(0,0,0,0,0,"global_state.modemanager.state = 2",0,0,0);
	trans[4][221]	= settr(764,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][244]	= settr(787,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[4][222]	= settr(765,2,235,498,0,"((global_state.modemanager.stc.selection==3))", 1, 2500, 0);
	T = trans[ 4][235] = settr(778,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(778,0,226,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[ 4][226] = settr(769,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(769,0,223,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][223]	= settr(766,2,225,499,499,"global_state.modemanager.stm.status1 = global_state.modemanager.status1", 1, 2500, 0);
	T = trans[ 4][225] = settr(768,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(768,0,224,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][224]	= settr(767,2,230,500,0,"assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))", 1, 2500, 0);
	T = trans[ 4][230] = settr(773,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(773,0,227,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][227]	= settr(770,2,229,501,501,"global_state.modemanager.stm.status2 = global_state.modemanager.status2", 1, 2500, 0);
	T = trans[ 4][229] = settr(772,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(772,0,228,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][228]	= settr(771,2,234,502,0,"assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))", 1, 2500, 0);
	T = trans[ 4][234] = settr(777,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(777,0,231,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][231]	= settr(774,2,233,503,503,"global_state.modemanager.stm.reading = global_state.modemanager.r", 1, 2500, 0);
	T = trans[ 4][233] = settr(776,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(776,0,232,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][232]	= settr(775,2,237,504,0,"assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))", 1, 2500, 0);
	T = trans[ 4][237] = settr(780,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(780,0,236,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][236]	= settr(779,2,300,505,505,"Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2", 1, 2500, 0); /* m: 238 -> 300,0 */
	reached4[238] = 1;
	trans[4][238]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][239]	= settr(782,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][240]	= settr(783,2,241,2,0,"else", 1, 2500, 0);
	trans[4][241]	= settr(784,2,300,506,506,"transition_id = -(1)", 1, 2500, 0);
	trans[4][242]	= settr(785,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][245]	= settr(788,2,247,507,507,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 4][247] = settr(790,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(790,0,246,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][246]	= settr(789,2,254,1,0,"(1)", 1, 2500, 0);
	T = trans[4][254] = settr(797,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(797,2,248,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(797,2,251,0,0,"IF", 1, 2500, 0);
	trans[4][248]	= settr(791,2,250,508,0,"((global_state.modemanager.r<global_state.modemanager.threshold1))", 1, 2500, 0);
	T = trans[ 4][250] = settr(793,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(793,0,249,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][249]	= settr(792,2,262,509,509,"Actuator1_actuatorTc_channel!1", 1, 2500, 0);
	trans[4][255]	= settr(798,2,262,1,0,".(goto)", 1, 2500, 0);
	trans[4][251]	= settr(794,2,253,2,0,"else", 1, 2500, 0);
	T = trans[ 4][253] = settr(796,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(796,0,252,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][252]	= settr(795,2,262,510,510,"Actuator1_actuatorTc_channel!2", 1, 2500, 0);
	T = trans[4][262] = settr(805,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(805,2,256,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(805,2,259,0,0,"IF", 1, 2500, 0);
	trans[4][256]	= settr(799,2,258,511,0,"((global_state.modemanager.r>global_state.modemanager.threshold2))", 1, 2500, 0);
	T = trans[ 4][258] = settr(801,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(801,0,257,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][257]	= settr(800,2,300,512,512,"Actuator2_actuatorTc_channel!1", 1, 2500, 0); /* m: 264 -> 300,0 */
	reached4[264] = 1;
	trans[4][263]	= settr(806,2,264,1,0,".(goto)", 1, 2500, 0); /* m: 264 -> 0,300 */
	reached4[264] = 1;
	trans[4][259]	= settr(802,2,261,2,0,"else", 1, 2500, 0);
	T = trans[ 4][261] = settr(804,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(804,0,260,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][260]	= settr(803,2,300,513,513,"Actuator2_actuatorTc_channel!2", 1, 2500, 0); /* m: 264 -> 300,0 */
	reached4[264] = 1;
	trans[4][264]	= settr(807,2,300,514,514,"transition_id = -(1)", 1, 2500, 0);
	trans[4][265]	= settr(808,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][266]	= settr(809,2,268,515,515,"((transition_id==4))", 1, 2500, 0);
	T = trans[ 4][268] = settr(811,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(811,0,267,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][267]	= settr(810,2,280,1,0,"(1)", 1, 2500, 0);
	trans[4][269]	= settr(812,2,280,1,0,"goto startcheck", 1, 2500, 0);
	trans[4][270]	= settr(813,2,272,516,516,"((transition_id==5))", 1, 2500, 0);
	T = trans[ 4][272] = settr(815,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(815,0,271,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][271]	= settr(814,2,280,1,0,"(1)", 1, 2500, 0);
	T = trans[4][280] = settr(823,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(823,2,273,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(823,2,276,0,0,"IF", 1, 2500, 0);
	trans[4][273]	= settr(816,2,300,517,517,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))", 1, 2500, 0); /* m: 274 -> 300,0 */
	reached4[274] = 1;
	trans[4][274]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][275]	= settr(818,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][281]	= settr(824,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[4][276]	= settr(819,2,300,518,518,"((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))", 1, 2500, 0); /* m: 277 -> 300,0 */
	reached4[277] = 1;
	trans[4][277]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][278]	= settr(0,0,0,0,0,"global_state.modemanager.state = 1",0,0,0);
	trans[4][279]	= settr(822,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][282]	= settr(825,2,284,519,519,"((transition_id==6))", 1, 2500, 0);
	T = trans[ 4][284] = settr(827,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(827,0,283,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][283]	= settr(826,2,296,1,0,"(1)", 1, 2500, 0);
	trans[4][285]	= settr(828,2,296,1,0,"goto stopcheck", 1, 2500, 0);
	trans[4][286]	= settr(829,2,288,520,520,"((transition_id==7))", 1, 2500, 0);
	T = trans[ 4][288] = settr(831,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(831,0,287,0,0,"sub-sequence", 1, 2500, 0);
	trans[4][287]	= settr(830,2,296,1,0,"(1)", 1, 2500, 0);
	T = trans[4][296] = settr(839,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(839,2,289,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(839,2,292,0,0,"IF", 1, 2500, 0);
	trans[4][289]	= settr(832,2,300,521,521,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))", 1, 2500, 0); /* m: 290 -> 300,0 */
	reached4[290] = 1;
	trans[4][290]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][291]	= settr(834,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][297]	= settr(840,2,300,1,0,".(goto)", 1, 2500, 0);
	trans[4][292]	= settr(835,2,300,522,522,"((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))", 1, 2500, 0); /* m: 293 -> 300,0 */
	reached4[293] = 1;
	trans[4][293]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[4][294]	= settr(0,0,0,0,0,"global_state.modemanager.state = 3",0,0,0);
	trans[4][295]	= settr(838,2,300,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[4][300]	= settr(843,2,301,1,0,"(1)", 1, 2500, 0);
	trans[4][303]	= settr(846,2,313,1,0,"break", 1, 2500, 0);
	trans[4][305]	= settr(848,2,306,2,0,"else", 1, 2500, 0);
	trans[4][306]	= settr(849,2,313,1,0,"(1)", 1, 2500, 0);
	trans[4][310]	= settr(853,2,313,1,0,"goto Modemanager_actuatortm1_loop", 1, 2500, 0);
	trans[4][314]	= settr(857,2,315,1,0,".(goto)", 1, 2500, 0);
	trans[4][311]	= settr(854,2,312,523,0,"(empty(Modemanager_actuatorTm1_channel))", 1, 2500, 0);
	trans[4][312]	= settr(855,2,315,524,524,"(1)", 1, 2500, 0); /* m: 314 -> 315,0 */
	reached4[314] = 1;
	trans[4][315]	= settr(858,0,317,525,525,"Modemanager_lock!1", 1, 2500, 0);
	trans[4][319]	= settr(862,0,320,1,0,"break", 0, 2, 0);
	trans[4][320]	= settr(863,0,0,526,526,"-end-", 0, 3500, 0);

	/* proctype 3: Egse_systemTc */

	trans[3] = (Trans **) emalloc(78*sizeof(Trans *));

	trans[3][1]	= settr(467,0,2,527,0,"(inited)", 1, 2, 0);
	trans[3][2]	= settr(468,0,74,528,528,"", 0, 2, 0); /* m: 3 -> 0,74 */
	reached3[3] = 1;
	trans[3][3]	= settr(0,0,0,0,0,"inputVectorCounter = 0",0,0,0);
	trans[3][4]	= settr(0,0,0,0,0,"inputVectorCounter = 0",0,0,0);
	trans[3][75]	= settr(541,0,74,1,0,".(goto)", 0, 2, 0);
	T = trans[3][74] = settr(540,0,0,0,0,"DO", 0, 2, 0);
	T = T->nxt	= settr(540,0,5,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(540,0,72,0,0,"DO", 0, 2, 0);
	trans[3][5]	= settr(471,0,70,529,0,"((inputVectorCounter<=3))", 0, 2, 0);
	T = trans[ 3][70] = settr(536,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(536,2,67,0,0,"ATOMIC", 1, 2, 0);
	T = trans[ 3][67] = settr(533,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(533,0,65,0,0,"sub-sequence", 1, 2, 0);
	T = trans[3][65] = settr(531,2,0,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(531,2,6,0,0,"IF", 1, 2, 0);
	T = T->nxt	= settr(531,2,21,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(531,2,50,0,0,"IF", 1, 2, 0);
	trans[3][6]	= settr(472,2,19,1,0,"(1)", 1, 2, 0);
	T = trans[ 3][19] = settr(485,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(485,0,7,0,0,"sub-sequence", 1, 2, 0);
	trans[3][7]	= settr(473,2,16,530,530,"Dummy_tmp = 0", 1, 2, 0);
	T = trans[3][16] = settr(482,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(482,2,15,0,0,"IF", 1, 2, 0);
	T = trans[ 3][15] = settr(481,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(481,0,8,0,0,"sub-sequence", 1, 2, 0);
	trans[3][8]	= settr(474,2,12,531,531,"Dummy_tmp = 0", 1, 2, 0);
	trans[3][13]	= settr(479,2,12,1,0,".(goto)", 1, 2, 0);
	T = trans[3][12] = settr(478,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(478,2,11,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(478,2,9,0,0,"DO", 1, 2, 0);
	trans[3][11]	= settr(477,2,69,532,532,"goto :b10", 1, 2, 0); /* m: 18 -> 0,69 */
	reached3[18] = 1;
	trans[3][9]	= settr(475,2,12,533,533,"((Dummy_tmp<0))", 1, 2, 0); /* m: 10 -> 12,0 */
	reached3[10] = 1;
	trans[3][10]	= settr(0,0,0,0,0,"Dummy_tmp = (Dummy_tmp+1)",0,0,0);
	trans[3][14]	= settr(480,2,18,1,0,"break", 1, 2, 0);
	trans[3][17]	= settr(483,2,18,1,0,".(goto)", 1, 2, 0); /* m: 18 -> 0,69 */
	reached3[18] = 1;
	trans[3][18]	= settr(484,2,69,534,534,"value.data.goToIdle = Dummy_tmp", 1, 2, 0); /* m: 20 -> 0,69 */
	reached3[20] = 1;
	trans[3][20]	= settr(0,0,0,0,0,"value.selection = 1",0,0,0);
	trans[3][66]	= settr(532,2,69,1,0,".(goto)", 1, 2, 0);
	trans[3][21]	= settr(487,2,48,1,0,"(1)", 1, 2, 0);
	T = trans[ 3][48] = settr(514,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(514,0,34,0,0,"sub-sequence", 1, 2, 0);
	T = trans[ 3][34] = settr(500,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(500,0,22,0,0,"sub-sequence", 1, 2, 0);
	trans[3][22]	= settr(488,2,31,535,535,"DataItem_tmp = 0", 1, 2, 0);
	T = trans[3][31] = settr(497,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(497,2,30,0,0,"IF", 1, 2, 0);
	T = trans[ 3][30] = settr(496,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(496,0,23,0,0,"sub-sequence", 1, 2, 0);
	trans[3][23]	= settr(489,2,27,536,536,"DataItem_tmp = 0", 1, 2, 0);
	trans[3][28]	= settr(494,2,27,1,0,".(goto)", 1, 2, 0);
	T = trans[3][27] = settr(493,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(493,2,26,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(493,2,24,0,0,"DO", 1, 2, 0);
	trans[3][26]	= settr(492,2,47,537,537,"goto :b11", 1, 2, 0); /* m: 33 -> 0,47 */
	reached3[33] = 1;
	trans[3][24]	= settr(490,2,27,538,538,"((DataItem_tmp<255))", 1, 2, 0); /* m: 25 -> 27,0 */
	reached3[25] = 1;
	trans[3][25]	= settr(0,0,0,0,0,"DataItem_tmp = (DataItem_tmp+1)",0,0,0);
	trans[3][29]	= settr(495,2,33,1,0,"break", 1, 2, 0);
	trans[3][32]	= settr(498,2,33,1,0,".(goto)", 1, 2, 0); /* m: 33 -> 0,47 */
	reached3[33] = 1;
	trans[3][33]	= settr(499,2,47,539,539,"value.data.goToOperational.threshold1 = DataItem_tmp", 1, 2, 0);
	T = trans[ 3][47] = settr(513,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(513,0,35,0,0,"sub-sequence", 1, 2, 0);
	trans[3][35]	= settr(501,2,44,540,540,"DataItem_tmp = 0", 1, 2, 0);
	T = trans[3][44] = settr(510,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(510,2,43,0,0,"IF", 1, 2, 0);
	T = trans[ 3][43] = settr(509,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(509,0,36,0,0,"sub-sequence", 1, 2, 0);
	trans[3][36]	= settr(502,2,40,541,541,"DataItem_tmp = 0", 1, 2, 0);
	trans[3][41]	= settr(507,2,40,1,0,".(goto)", 1, 2, 0);
	T = trans[3][40] = settr(506,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(506,2,39,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(506,2,37,0,0,"DO", 1, 2, 0);
	trans[3][39]	= settr(505,2,69,542,542,"goto :b12", 1, 2, 0); /* m: 46 -> 0,69 */
	reached3[46] = 1;
	trans[3][37]	= settr(503,2,40,543,543,"((DataItem_tmp<255))", 1, 2, 0); /* m: 38 -> 40,0 */
	reached3[38] = 1;
	trans[3][38]	= settr(0,0,0,0,0,"DataItem_tmp = (DataItem_tmp+1)",0,0,0);
	trans[3][42]	= settr(508,2,46,1,0,"break", 1, 2, 0);
	trans[3][45]	= settr(511,2,46,1,0,".(goto)", 1, 2, 0); /* m: 46 -> 0,69 */
	reached3[46] = 1;
	trans[3][46]	= settr(512,2,69,544,544,"value.data.goToOperational.threshold2 = DataItem_tmp", 1, 2, 0); /* m: 49 -> 0,69 */
	reached3[49] = 1;
	trans[3][49]	= settr(0,0,0,0,0,"value.selection = 2",0,0,0);
	trans[3][50]	= settr(516,2,63,1,0,"(1)", 1, 2, 0);
	T = trans[ 3][63] = settr(529,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(529,0,51,0,0,"sub-sequence", 1, 2, 0);
	trans[3][51]	= settr(517,2,60,545,545,"Dummy_tmp = 0", 1, 2, 0);
	T = trans[3][60] = settr(526,2,0,0,0,"IF", 1, 2, 0);
	    T->nxt	= settr(526,2,59,0,0,"IF", 1, 2, 0);
	T = trans[ 3][59] = settr(525,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(525,0,52,0,0,"sub-sequence", 1, 2, 0);
	trans[3][52]	= settr(518,2,56,546,546,"Dummy_tmp = 0", 1, 2, 0);
	trans[3][57]	= settr(523,2,56,1,0,".(goto)", 1, 2, 0);
	T = trans[3][56] = settr(522,2,0,0,0,"DO", 1, 2, 0);
	T = T->nxt	= settr(522,2,55,0,0,"DO", 1, 2, 0);
	    T->nxt	= settr(522,2,53,0,0,"DO", 1, 2, 0);
	trans[3][55]	= settr(521,2,69,547,547,"goto :b13", 1, 2, 0); /* m: 62 -> 0,69 */
	reached3[62] = 1;
	trans[3][53]	= settr(519,2,56,548,548,"((Dummy_tmp<0))", 1, 2, 0); /* m: 54 -> 56,0 */
	reached3[54] = 1;
	trans[3][54]	= settr(0,0,0,0,0,"Dummy_tmp = (Dummy_tmp+1)",0,0,0);
	trans[3][58]	= settr(524,2,62,1,0,"break", 1, 2, 0);
	trans[3][61]	= settr(527,2,62,1,0,".(goto)", 1, 2, 0); /* m: 62 -> 0,69 */
	reached3[62] = 1;
	trans[3][62]	= settr(528,2,69,549,549,"value.data.doReport = Dummy_tmp", 1, 2, 0); /* m: 64 -> 0,69 */
	reached3[64] = 1;
	trans[3][64]	= settr(0,0,0,0,0,"value.selection = 3",0,0,0);
	T = trans[ 3][69] = settr(535,0,0,0,0,"sub-sequence", 1, 2, 0);
	T->nxt	= settr(535,0,68,0,0,"sub-sequence", 1, 2, 0);
	trans[3][68]	= settr(534,0,74,550,550,"Modemanager_systemTc_channel!value.data.goToIdle,value.data.goToOperational.threshold1,value.data.goToOperational.threshold2,value.data.doReport,value.selection", 1, 9, 0); /* m: 71 -> 74,0 */
	reached3[71] = 1;
	trans[3][71]	= settr(0,0,0,0,0,"inputVectorCounter = (inputVectorCounter+1)",0,0,0);
	trans[3][72]	= settr(538,0,77,2,0,"else", 0, 2, 0);
	trans[3][73]	= settr(539,0,77,1,0,"goto :b9", 0, 2, 0);
	trans[3][76]	= settr(542,0,77,1,0,"break", 0, 2, 0);
	trans[3][77]	= settr(543,0,0,551,551,"-end-", 0, 3500, 0);

	/* proctype 2: Egse_systemTm */

	trans[2] = (Trans **) emalloc(16*sizeof(Trans *));

	trans[2][1]	= settr(452,0,12,552,0,"(inited)", 1, 2, 0);
	trans[2][13]	= settr(464,0,12,1,0,".(goto)", 0, 2, 0);
	T = trans[2][12] = settr(463,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(463,0,11,0,0,"DO", 0, 2, 0);
	T = trans[ 2][11] = settr(462,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(462,2,2,0,0,"ATOMIC", 1, 505, 1005);
	trans[2][2]	= settr(453,2,9,553,0,"(nempty(Egse_systemTm_channel))", 1, 505, 1005);
	T = trans[2][9] = settr(460,2,0,0,0,"IF", 1, 505, 1005);
	T = T->nxt	= settr(460,2,3,0,0,"IF", 1, 505, 1005);
	    T->nxt	= settr(460,2,7,0,0,"IF", 1, 505, 1005);
	trans[2][3]	= settr(454,2,4,554,0,"(nempty(Egse_systemTm_channel))", 1, 505, 1005);
	trans[2][4]	= settr(455,2,9,555,555,"Egse_systemTm_channel?Egse_systemtm_signal_parameter.reading,Egse_systemtm_signal_parameter.status1,Egse_systemtm_signal_parameter.status2", 1, 505, 1005); /* m: 5 -> 9,0 */
	reached2[5] = 1;
	trans[2][5]	= settr(0,0,0,0,0,"Egse_systemtm_channel_used = 1",0,0,0);
	trans[2][6]	= settr(457,2,9,1,0,"goto Egse_systemtm_loop", 1, 505, 1005);
	trans[2][10]	= settr(461,0,12,1,0,".(goto)", 1, 505, 1005);
	trans[2][7]	= settr(458,2,8,556,0,"(empty(Egse_systemTm_channel))", 1, 505, 1005);
	trans[2][8]	= settr(459,0,12,557,557,"(1)", 1, 505, 1005); /* m: 10 -> 12,0 */
	reached2[10] = 1;
	trans[2][14]	= settr(465,0,15,1,0,"break", 0, 2, 0);
	trans[2][15]	= settr(466,0,0,558,558,"-end-", 0, 3500, 0);

	/* proctype 1: Actuator2_actuatorTc */

	trans[1] = (Trans **) emalloc(227*sizeof(Trans *));

	trans[1][1]	= settr(226,0,223,559,0,"(inited)", 1, 2, 0);
	trans[1][224]	= settr(449,0,223,1,0,".(goto)", 0, 2, 0);
	T = trans[1][223] = settr(448,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(448,0,222,0,0,"DO", 0, 2, 0);
	T = trans[ 1][222] = settr(447,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(447,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[1][2]	= settr(227,2,3,560,0,"(nempty(Actuator2_actuatorTc_channel))", 1, 2500, 0);
	trans[1][3]	= settr(228,2,219,561,561,"Actuator2_lock?_", 1, 2500, 0);
	T = trans[1][219] = settr(444,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(444,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(444,2,217,0,0,"IF", 1, 2500, 0);
	trans[1][4]	= settr(229,2,5,562,0,"(nempty(Actuator2_actuatorTc_channel))", 1, 2500, 0);
	trans[1][5]	= settr(230,2,215,563,563,"Actuator2_actuatorTc_channel?Actuator2_actuatortc_signal_parameter", 1, 2500, 0); /* m: 6 -> 215,0 */
	reached1[6] = 1;
	trans[1][6]	= settr(0,0,0,0,0,"Actuator2_actuatortc_channel_used = 1",0,0,0);
	T = trans[ 1][215] = settr(440,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(440,0,213,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[1][213] = settr(438,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(438,2,7,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(438,2,75,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(438,2,143,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(438,2,211,0,0,"IF", 1, 2500, 0);
	trans[1][7]	= settr(232,2,11,564,0,"((global_state.actuator2.state==0))", 1, 2500, 0);
	T = trans[ 1][11] = settr(236,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(236,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][8]	= settr(233,2,10,565,565,"global_state.actuator2.cmd = Actuator2_actuatortc_signal_parameter", 1, 2500, 0);
	T = trans[ 1][10] = settr(235,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(235,0,9,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][9]	= settr(234,2,74,566,0,"assert((((global_state.actuator2.cmd==0)||(global_state.actuator2.cmd==1))||(global_state.actuator2.cmd==2)))", 1, 2500, 0);
	T = trans[ 1][74] = settr(299,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(299,0,12,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][12]	= settr(237,2,71,567,567,"transition_id = 0", 1, 2500, 0); /* m: 13 -> 0,71 */
	reached1[13] = 1;
	trans[1][13]	= settr(0,0,0,0,0,"transition_id = 1",0,0,0);
	trans[1][72]	= settr(297,2,71,1,0,".(goto)", 1, 2500, 0);
	T = trans[1][71] = settr(296,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(296,2,68,0,0,"DO", 1, 2500, 0);
	T = trans[1][68] = settr(293,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(293,2,14,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(293,2,16,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(293,2,20,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(293,2,40,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(293,2,54,0,0,"IF", 1, 2500, 0);
	trans[1][14]	= settr(239,2,219,568,568,"((transition_id==-(1)))", 1, 2500, 0);
	trans[1][15]	= settr(240,2,219,1,0,"goto :b5", 1, 2500, 0);
	trans[1][69]	= settr(294,2,70,1,0,".(goto)", 1, 2500, 0);
	trans[1][16]	= settr(241,2,70,569,569,"((transition_id==0))", 1, 2500, 0); /* m: 17 -> 70,0 */
	reached1[17] = 1;
	trans[1][17]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][18]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][19]	= settr(244,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][20]	= settr(245,2,22,570,570,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 1][22] = settr(247,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(247,0,21,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][21]	= settr(246,2,38,1,0,"(1)", 1, 2500, 0);
	T = trans[1][38] = settr(263,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(263,2,23,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(263,2,29,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(263,2,35,0,0,"IF", 1, 2500, 0);
	trans[1][23]	= settr(248,2,25,571,0,"((global_state.actuator2.cmd==2))", 1, 2500, 0);
	T = trans[ 1][25] = settr(250,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(250,0,24,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][24]	= settr(249,2,70,572,572,"Modemanager_actuatorTm2_channel!2", 1, 2500, 0); /* m: 26 -> 70,0 */
	reached1[26] = 1;
	trans[1][26]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][27]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][28]	= settr(253,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][39]	= settr(264,2,70,1,0,".(goto)", 1, 2500, 0);
	trans[1][29]	= settr(254,2,31,573,0,"((global_state.actuator2.cmd==1))", 1, 2500, 0);
	T = trans[ 1][31] = settr(256,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(256,0,30,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][30]	= settr(255,2,70,574,574,"Modemanager_actuatorTm2_channel!1", 1, 2500, 0); /* m: 32 -> 70,0 */
	reached1[32] = 1;
	trans[1][32]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][33]	= settr(0,0,0,0,0,"global_state.actuator2.state = 1",0,0,0);
	trans[1][34]	= settr(259,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][35]	= settr(260,2,36,2,0,"else", 1, 2500, 0);
	trans[1][36]	= settr(261,2,70,575,575,"transition_id = -(1)", 1, 2500, 0);
	trans[1][37]	= settr(262,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][40]	= settr(265,2,42,576,576,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 1][42] = settr(267,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(267,0,41,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][41]	= settr(266,2,52,1,0,"(1)", 1, 2500, 0);
	T = trans[1][52] = settr(277,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(277,2,43,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(277,2,49,0,0,"IF", 1, 2500, 0);
	trans[1][43]	= settr(268,2,45,577,0,"((global_state.actuator2.cmd==2))", 1, 2500, 0);
	T = trans[ 1][45] = settr(270,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(270,0,44,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][44]	= settr(269,2,70,578,578,"Modemanager_actuatorTm2_channel!2", 1, 2500, 0); /* m: 46 -> 70,0 */
	reached1[46] = 1;
	trans[1][46]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][47]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][48]	= settr(273,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][53]	= settr(278,2,70,1,0,".(goto)", 1, 2500, 0);
	trans[1][49]	= settr(274,2,50,2,0,"else", 1, 2500, 0);
	trans[1][50]	= settr(275,2,70,579,579,"transition_id = -(1)", 1, 2500, 0);
	trans[1][51]	= settr(276,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][54]	= settr(279,2,56,580,580,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 1][56] = settr(281,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(281,0,55,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][55]	= settr(280,2,66,1,0,"(1)", 1, 2500, 0);
	T = trans[1][66] = settr(291,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(291,2,57,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(291,2,63,0,0,"IF", 1, 2500, 0);
	trans[1][57]	= settr(282,2,59,581,0,"((global_state.actuator2.cmd==0))", 1, 2500, 0);
	T = trans[ 1][59] = settr(284,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(284,0,58,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][58]	= settr(283,2,70,582,582,"Modemanager_actuatorTm2_channel!0", 1, 2500, 0); /* m: 60 -> 70,0 */
	reached1[60] = 1;
	trans[1][60]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][61]	= settr(0,0,0,0,0,"global_state.actuator2.state = 0",0,0,0);
	trans[1][62]	= settr(287,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][67]	= settr(292,2,70,1,0,".(goto)", 1, 2500, 0);
	trans[1][63]	= settr(288,2,64,2,0,"else", 1, 2500, 0);
	trans[1][64]	= settr(289,2,70,583,583,"transition_id = -(1)", 1, 2500, 0);
	trans[1][65]	= settr(290,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][70]	= settr(295,2,71,1,0,"(1)", 1, 2500, 0);
	trans[1][73]	= settr(298,2,219,1,0,"break", 1, 2500, 0);
	trans[1][214]	= settr(439,2,219,1,0,".(goto)", 1, 2500, 0);
	trans[1][75]	= settr(300,2,79,584,0,"((global_state.actuator2.state==1))", 1, 2500, 0);
	T = trans[ 1][79] = settr(304,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(304,0,76,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][76]	= settr(301,2,78,585,585,"global_state.actuator2.cmd = Actuator2_actuatortc_signal_parameter", 1, 2500, 0);
	T = trans[ 1][78] = settr(303,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(303,0,77,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][77]	= settr(302,2,142,586,0,"assert((((global_state.actuator2.cmd==0)||(global_state.actuator2.cmd==1))||(global_state.actuator2.cmd==2)))", 1, 2500, 0);
	T = trans[ 1][142] = settr(367,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(367,0,80,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][80]	= settr(305,2,139,587,587,"transition_id = 0", 1, 2500, 0); /* m: 81 -> 0,139 */
	reached1[81] = 1;
	trans[1][81]	= settr(0,0,0,0,0,"transition_id = 2",0,0,0);
	trans[1][140]	= settr(365,2,139,1,0,".(goto)", 1, 2500, 0);
	T = trans[1][139] = settr(364,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(364,2,136,0,0,"DO", 1, 2500, 0);
	T = trans[1][136] = settr(361,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(361,2,82,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(361,2,84,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(361,2,88,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(361,2,108,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(361,2,122,0,0,"IF", 1, 2500, 0);
	trans[1][82]	= settr(307,2,219,588,588,"((transition_id==-(1)))", 1, 2500, 0);
	trans[1][83]	= settr(308,2,219,1,0,"goto :b6", 1, 2500, 0);
	trans[1][137]	= settr(362,2,138,1,0,".(goto)", 1, 2500, 0);
	trans[1][84]	= settr(309,2,138,589,589,"((transition_id==0))", 1, 2500, 0); /* m: 85 -> 138,0 */
	reached1[85] = 1;
	trans[1][85]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][86]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][87]	= settr(312,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][88]	= settr(313,2,90,590,590,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 1][90] = settr(315,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(315,0,89,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][89]	= settr(314,2,106,1,0,"(1)", 1, 2500, 0);
	T = trans[1][106] = settr(331,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(331,2,91,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(331,2,97,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(331,2,103,0,0,"IF", 1, 2500, 0);
	trans[1][91]	= settr(316,2,93,591,0,"((global_state.actuator2.cmd==2))", 1, 2500, 0);
	T = trans[ 1][93] = settr(318,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(318,0,92,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][92]	= settr(317,2,138,592,592,"Modemanager_actuatorTm2_channel!2", 1, 2500, 0); /* m: 94 -> 138,0 */
	reached1[94] = 1;
	trans[1][94]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][95]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][96]	= settr(321,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][107]	= settr(332,2,138,1,0,".(goto)", 1, 2500, 0);
	trans[1][97]	= settr(322,2,99,593,0,"((global_state.actuator2.cmd==1))", 1, 2500, 0);
	T = trans[ 1][99] = settr(324,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(324,0,98,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][98]	= settr(323,2,138,594,594,"Modemanager_actuatorTm2_channel!1", 1, 2500, 0); /* m: 100 -> 138,0 */
	reached1[100] = 1;
	trans[1][100]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][101]	= settr(0,0,0,0,0,"global_state.actuator2.state = 1",0,0,0);
	trans[1][102]	= settr(327,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][103]	= settr(328,2,104,2,0,"else", 1, 2500, 0);
	trans[1][104]	= settr(329,2,138,595,595,"transition_id = -(1)", 1, 2500, 0);
	trans[1][105]	= settr(330,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][108]	= settr(333,2,110,596,596,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 1][110] = settr(335,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(335,0,109,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][109]	= settr(334,2,120,1,0,"(1)", 1, 2500, 0);
	T = trans[1][120] = settr(345,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(345,2,111,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(345,2,117,0,0,"IF", 1, 2500, 0);
	trans[1][111]	= settr(336,2,113,597,0,"((global_state.actuator2.cmd==2))", 1, 2500, 0);
	T = trans[ 1][113] = settr(338,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(338,0,112,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][112]	= settr(337,2,138,598,598,"Modemanager_actuatorTm2_channel!2", 1, 2500, 0); /* m: 114 -> 138,0 */
	reached1[114] = 1;
	trans[1][114]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][115]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][116]	= settr(341,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][121]	= settr(346,2,138,1,0,".(goto)", 1, 2500, 0);
	trans[1][117]	= settr(342,2,118,2,0,"else", 1, 2500, 0);
	trans[1][118]	= settr(343,2,138,599,599,"transition_id = -(1)", 1, 2500, 0);
	trans[1][119]	= settr(344,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][122]	= settr(347,2,124,600,600,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 1][124] = settr(349,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(349,0,123,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][123]	= settr(348,2,134,1,0,"(1)", 1, 2500, 0);
	T = trans[1][134] = settr(359,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(359,2,125,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(359,2,131,0,0,"IF", 1, 2500, 0);
	trans[1][125]	= settr(350,2,127,601,0,"((global_state.actuator2.cmd==0))", 1, 2500, 0);
	T = trans[ 1][127] = settr(352,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(352,0,126,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][126]	= settr(351,2,138,602,602,"Modemanager_actuatorTm2_channel!0", 1, 2500, 0); /* m: 128 -> 138,0 */
	reached1[128] = 1;
	trans[1][128]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][129]	= settr(0,0,0,0,0,"global_state.actuator2.state = 0",0,0,0);
	trans[1][130]	= settr(355,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][135]	= settr(360,2,138,1,0,".(goto)", 1, 2500, 0);
	trans[1][131]	= settr(356,2,132,2,0,"else", 1, 2500, 0);
	trans[1][132]	= settr(357,2,138,603,603,"transition_id = -(1)", 1, 2500, 0);
	trans[1][133]	= settr(358,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][138]	= settr(363,2,139,1,0,"(1)", 1, 2500, 0);
	trans[1][141]	= settr(366,2,219,1,0,"break", 1, 2500, 0);
	trans[1][143]	= settr(368,2,147,604,0,"((global_state.actuator2.state==2))", 1, 2500, 0);
	T = trans[ 1][147] = settr(372,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(372,0,144,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][144]	= settr(369,2,146,605,605,"global_state.actuator2.cmd = Actuator2_actuatortc_signal_parameter", 1, 2500, 0);
	T = trans[ 1][146] = settr(371,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(371,0,145,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][145]	= settr(370,2,210,606,0,"assert((((global_state.actuator2.cmd==0)||(global_state.actuator2.cmd==1))||(global_state.actuator2.cmd==2)))", 1, 2500, 0);
	T = trans[ 1][210] = settr(435,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(435,0,148,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][148]	= settr(373,2,207,607,607,"transition_id = 0", 1, 2500, 0); /* m: 149 -> 0,207 */
	reached1[149] = 1;
	trans[1][149]	= settr(0,0,0,0,0,"transition_id = 3",0,0,0);
	trans[1][208]	= settr(433,2,207,1,0,".(goto)", 1, 2500, 0);
	T = trans[1][207] = settr(432,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(432,2,204,0,0,"DO", 1, 2500, 0);
	T = trans[1][204] = settr(429,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(429,2,150,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(429,2,152,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(429,2,156,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(429,2,176,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(429,2,190,0,0,"IF", 1, 2500, 0);
	trans[1][150]	= settr(375,2,219,608,608,"((transition_id==-(1)))", 1, 2500, 0);
	trans[1][151]	= settr(376,2,219,1,0,"goto :b7", 1, 2500, 0);
	trans[1][205]	= settr(430,2,206,1,0,".(goto)", 1, 2500, 0);
	trans[1][152]	= settr(377,2,206,609,609,"((transition_id==0))", 1, 2500, 0); /* m: 153 -> 206,0 */
	reached1[153] = 1;
	trans[1][153]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][154]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][155]	= settr(380,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][156]	= settr(381,2,158,610,610,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 1][158] = settr(383,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(383,0,157,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][157]	= settr(382,2,174,1,0,"(1)", 1, 2500, 0);
	T = trans[1][174] = settr(399,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(399,2,159,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(399,2,165,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(399,2,171,0,0,"IF", 1, 2500, 0);
	trans[1][159]	= settr(384,2,161,611,0,"((global_state.actuator2.cmd==2))", 1, 2500, 0);
	T = trans[ 1][161] = settr(386,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(386,0,160,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][160]	= settr(385,2,206,612,612,"Modemanager_actuatorTm2_channel!2", 1, 2500, 0); /* m: 162 -> 206,0 */
	reached1[162] = 1;
	trans[1][162]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][163]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][164]	= settr(389,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][175]	= settr(400,2,206,1,0,".(goto)", 1, 2500, 0);
	trans[1][165]	= settr(390,2,167,613,0,"((global_state.actuator2.cmd==1))", 1, 2500, 0);
	T = trans[ 1][167] = settr(392,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(392,0,166,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][166]	= settr(391,2,206,614,614,"Modemanager_actuatorTm2_channel!1", 1, 2500, 0); /* m: 168 -> 206,0 */
	reached1[168] = 1;
	trans[1][168]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][169]	= settr(0,0,0,0,0,"global_state.actuator2.state = 1",0,0,0);
	trans[1][170]	= settr(395,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][171]	= settr(396,2,172,2,0,"else", 1, 2500, 0);
	trans[1][172]	= settr(397,2,206,615,615,"transition_id = -(1)", 1, 2500, 0);
	trans[1][173]	= settr(398,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][176]	= settr(401,2,178,616,616,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 1][178] = settr(403,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(403,0,177,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][177]	= settr(402,2,188,1,0,"(1)", 1, 2500, 0);
	T = trans[1][188] = settr(413,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(413,2,179,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(413,2,185,0,0,"IF", 1, 2500, 0);
	trans[1][179]	= settr(404,2,181,617,0,"((global_state.actuator2.cmd==2))", 1, 2500, 0);
	T = trans[ 1][181] = settr(406,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(406,0,180,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][180]	= settr(405,2,206,618,618,"Modemanager_actuatorTm2_channel!2", 1, 2500, 0); /* m: 182 -> 206,0 */
	reached1[182] = 1;
	trans[1][182]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][183]	= settr(0,0,0,0,0,"global_state.actuator2.state = 2",0,0,0);
	trans[1][184]	= settr(409,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][189]	= settr(414,2,206,1,0,".(goto)", 1, 2500, 0);
	trans[1][185]	= settr(410,2,186,2,0,"else", 1, 2500, 0);
	trans[1][186]	= settr(411,2,206,619,619,"transition_id = -(1)", 1, 2500, 0);
	trans[1][187]	= settr(412,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][190]	= settr(415,2,192,620,620,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 1][192] = settr(417,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(417,0,191,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][191]	= settr(416,2,202,1,0,"(1)", 1, 2500, 0);
	T = trans[1][202] = settr(427,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(427,2,193,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(427,2,199,0,0,"IF", 1, 2500, 0);
	trans[1][193]	= settr(418,2,195,621,0,"((global_state.actuator2.cmd==0))", 1, 2500, 0);
	T = trans[ 1][195] = settr(420,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(420,0,194,0,0,"sub-sequence", 1, 2500, 0);
	trans[1][194]	= settr(419,2,206,622,622,"Modemanager_actuatorTm2_channel!0", 1, 2500, 0); /* m: 196 -> 206,0 */
	reached1[196] = 1;
	trans[1][196]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[1][197]	= settr(0,0,0,0,0,"global_state.actuator2.state = 0",0,0,0);
	trans[1][198]	= settr(423,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][203]	= settr(428,2,206,1,0,".(goto)", 1, 2500, 0);
	trans[1][199]	= settr(424,2,200,2,0,"else", 1, 2500, 0);
	trans[1][200]	= settr(425,2,206,623,623,"transition_id = -(1)", 1, 2500, 0);
	trans[1][201]	= settr(426,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[1][206]	= settr(431,2,207,1,0,"(1)", 1, 2500, 0);
	trans[1][209]	= settr(434,2,219,1,0,"break", 1, 2500, 0);
	trans[1][211]	= settr(436,2,212,2,0,"else", 1, 2500, 0);
	trans[1][212]	= settr(437,2,219,1,0,"(1)", 1, 2500, 0);
	trans[1][216]	= settr(441,2,219,1,0,"goto Actuator2_actuatortc_loop", 1, 2500, 0);
	trans[1][220]	= settr(445,2,221,1,0,".(goto)", 1, 2500, 0);
	trans[1][217]	= settr(442,2,218,624,0,"(empty(Actuator2_actuatorTc_channel))", 1, 2500, 0);
	trans[1][218]	= settr(443,2,221,625,625,"(1)", 1, 2500, 0); /* m: 220 -> 221,0 */
	reached1[220] = 1;
	trans[1][221]	= settr(446,0,223,626,626,"Actuator2_lock!1", 1, 2500, 0);
	trans[1][225]	= settr(450,0,226,1,0,"break", 0, 2, 0);
	trans[1][226]	= settr(451,0,0,627,627,"-end-", 0, 3500, 0);

	/* proctype 0: Actuator1_actuatorTc */

	trans[0] = (Trans **) emalloc(227*sizeof(Trans *));

	trans[0][1]	= settr(0,0,223,628,0,"(inited)", 1, 2, 0);
	trans[0][224]	= settr(223,0,223,1,0,".(goto)", 0, 2, 0);
	T = trans[0][223] = settr(222,0,0,0,0,"DO", 0, 2, 0);
	    T->nxt	= settr(222,0,222,0,0,"DO", 0, 2, 0);
	T = trans[ 0][222] = settr(221,2,0,0,0,"ATOMIC", 1, 2, 0);
	T->nxt	= settr(221,2,2,0,0,"ATOMIC", 1, 2500, 0);
	trans[0][2]	= settr(1,2,3,629,0,"(nempty(Actuator1_actuatorTc_channel))", 1, 2500, 0);
	trans[0][3]	= settr(2,2,219,630,630,"Actuator1_lock?_", 1, 2500, 0);
	T = trans[0][219] = settr(218,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(218,2,4,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(218,2,217,0,0,"IF", 1, 2500, 0);
	trans[0][4]	= settr(3,2,5,631,0,"(nempty(Actuator1_actuatorTc_channel))", 1, 2500, 0);
	trans[0][5]	= settr(4,2,215,632,632,"Actuator1_actuatorTc_channel?Actuator1_actuatortc_signal_parameter", 1, 2500, 0); /* m: 6 -> 215,0 */
	reached0[6] = 1;
	trans[0][6]	= settr(0,0,0,0,0,"Actuator1_actuatortc_channel_used = 1",0,0,0);
	T = trans[ 0][215] = settr(214,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(214,0,213,0,0,"sub-sequence", 1, 2500, 0);
	T = trans[0][213] = settr(212,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(212,2,7,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(212,2,75,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(212,2,143,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(212,2,211,0,0,"IF", 1, 2500, 0);
	trans[0][7]	= settr(6,2,11,633,0,"((global_state.actuator1.state==0))", 1, 2500, 0);
	T = trans[ 0][11] = settr(10,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(10,0,8,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][8]	= settr(7,2,10,634,634,"global_state.actuator1.cmd = Actuator1_actuatortc_signal_parameter", 1, 2500, 0);
	T = trans[ 0][10] = settr(9,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(9,0,9,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][9]	= settr(8,2,74,635,0,"assert((((global_state.actuator1.cmd==0)||(global_state.actuator1.cmd==1))||(global_state.actuator1.cmd==2)))", 1, 2500, 0);
	T = trans[ 0][74] = settr(73,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(73,0,12,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][12]	= settr(11,2,71,636,636,"transition_id = 0", 1, 2500, 0); /* m: 13 -> 0,71 */
	reached0[13] = 1;
	trans[0][13]	= settr(0,0,0,0,0,"transition_id = 1",0,0,0);
	trans[0][72]	= settr(71,2,71,1,0,".(goto)", 1, 2500, 0);
	T = trans[0][71] = settr(70,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(70,2,68,0,0,"DO", 1, 2500, 0);
	T = trans[0][68] = settr(67,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(67,2,14,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(67,2,16,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(67,2,20,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(67,2,40,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(67,2,54,0,0,"IF", 1, 2500, 0);
	trans[0][14]	= settr(13,2,219,637,637,"((transition_id==-(1)))", 1, 2500, 0);
	trans[0][15]	= settr(14,2,219,1,0,"goto :b1", 1, 2500, 0);
	trans[0][69]	= settr(68,2,70,1,0,".(goto)", 1, 2500, 0);
	trans[0][16]	= settr(15,2,70,638,638,"((transition_id==0))", 1, 2500, 0); /* m: 17 -> 70,0 */
	reached0[17] = 1;
	trans[0][17]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][18]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][19]	= settr(18,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][20]	= settr(19,2,22,639,639,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 0][22] = settr(21,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(21,0,21,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][21]	= settr(20,2,38,1,0,"(1)", 1, 2500, 0);
	T = trans[0][38] = settr(37,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(37,2,23,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(37,2,29,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(37,2,35,0,0,"IF", 1, 2500, 0);
	trans[0][23]	= settr(22,2,25,640,0,"((global_state.actuator1.cmd==2))", 1, 2500, 0);
	T = trans[ 0][25] = settr(24,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(24,0,24,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][24]	= settr(23,2,70,641,641,"Modemanager_actuatorTm1_channel!2", 1, 2500, 0); /* m: 26 -> 70,0 */
	reached0[26] = 1;
	trans[0][26]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][27]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][28]	= settr(27,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][39]	= settr(38,2,70,1,0,".(goto)", 1, 2500, 0);
	trans[0][29]	= settr(28,2,31,642,0,"((global_state.actuator1.cmd==1))", 1, 2500, 0);
	T = trans[ 0][31] = settr(30,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(30,0,30,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][30]	= settr(29,2,70,643,643,"Modemanager_actuatorTm1_channel!1", 1, 2500, 0); /* m: 32 -> 70,0 */
	reached0[32] = 1;
	trans[0][32]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][33]	= settr(0,0,0,0,0,"global_state.actuator1.state = 1",0,0,0);
	trans[0][34]	= settr(33,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][35]	= settr(34,2,36,2,0,"else", 1, 2500, 0);
	trans[0][36]	= settr(35,2,70,644,644,"transition_id = -(1)", 1, 2500, 0);
	trans[0][37]	= settr(36,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][40]	= settr(39,2,42,645,645,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 0][42] = settr(41,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(41,0,41,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][41]	= settr(40,2,52,1,0,"(1)", 1, 2500, 0);
	T = trans[0][52] = settr(51,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(51,2,43,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(51,2,49,0,0,"IF", 1, 2500, 0);
	trans[0][43]	= settr(42,2,45,646,0,"((global_state.actuator1.cmd==2))", 1, 2500, 0);
	T = trans[ 0][45] = settr(44,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(44,0,44,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][44]	= settr(43,2,70,647,647,"Modemanager_actuatorTm1_channel!2", 1, 2500, 0); /* m: 46 -> 70,0 */
	reached0[46] = 1;
	trans[0][46]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][47]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][48]	= settr(47,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][53]	= settr(52,2,70,1,0,".(goto)", 1, 2500, 0);
	trans[0][49]	= settr(48,2,50,2,0,"else", 1, 2500, 0);
	trans[0][50]	= settr(49,2,70,648,648,"transition_id = -(1)", 1, 2500, 0);
	trans[0][51]	= settr(50,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][54]	= settr(53,2,56,649,649,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 0][56] = settr(55,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(55,0,55,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][55]	= settr(54,2,66,1,0,"(1)", 1, 2500, 0);
	T = trans[0][66] = settr(65,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(65,2,57,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(65,2,63,0,0,"IF", 1, 2500, 0);
	trans[0][57]	= settr(56,2,59,650,0,"((global_state.actuator1.cmd==0))", 1, 2500, 0);
	T = trans[ 0][59] = settr(58,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(58,0,58,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][58]	= settr(57,2,70,651,651,"Modemanager_actuatorTm1_channel!0", 1, 2500, 0); /* m: 60 -> 70,0 */
	reached0[60] = 1;
	trans[0][60]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][61]	= settr(0,0,0,0,0,"global_state.actuator1.state = 0",0,0,0);
	trans[0][62]	= settr(61,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][67]	= settr(66,2,70,1,0,".(goto)", 1, 2500, 0);
	trans[0][63]	= settr(62,2,64,2,0,"else", 1, 2500, 0);
	trans[0][64]	= settr(63,2,70,652,652,"transition_id = -(1)", 1, 2500, 0);
	trans[0][65]	= settr(64,2,70,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][70]	= settr(69,2,71,1,0,"(1)", 1, 2500, 0);
	trans[0][73]	= settr(72,2,219,1,0,"break", 1, 2500, 0);
	trans[0][214]	= settr(213,2,219,1,0,".(goto)", 1, 2500, 0);
	trans[0][75]	= settr(74,2,79,653,0,"((global_state.actuator1.state==1))", 1, 2500, 0);
	T = trans[ 0][79] = settr(78,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(78,0,76,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][76]	= settr(75,2,78,654,654,"global_state.actuator1.cmd = Actuator1_actuatortc_signal_parameter", 1, 2500, 0);
	T = trans[ 0][78] = settr(77,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(77,0,77,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][77]	= settr(76,2,142,655,0,"assert((((global_state.actuator1.cmd==0)||(global_state.actuator1.cmd==1))||(global_state.actuator1.cmd==2)))", 1, 2500, 0);
	T = trans[ 0][142] = settr(141,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(141,0,80,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][80]	= settr(79,2,139,656,656,"transition_id = 0", 1, 2500, 0); /* m: 81 -> 0,139 */
	reached0[81] = 1;
	trans[0][81]	= settr(0,0,0,0,0,"transition_id = 2",0,0,0);
	trans[0][140]	= settr(139,2,139,1,0,".(goto)", 1, 2500, 0);
	T = trans[0][139] = settr(138,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(138,2,136,0,0,"DO", 1, 2500, 0);
	T = trans[0][136] = settr(135,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(135,2,82,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(135,2,84,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(135,2,88,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(135,2,108,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(135,2,122,0,0,"IF", 1, 2500, 0);
	trans[0][82]	= settr(81,2,219,657,657,"((transition_id==-(1)))", 1, 2500, 0);
	trans[0][83]	= settr(82,2,219,1,0,"goto :b2", 1, 2500, 0);
	trans[0][137]	= settr(136,2,138,1,0,".(goto)", 1, 2500, 0);
	trans[0][84]	= settr(83,2,138,658,658,"((transition_id==0))", 1, 2500, 0); /* m: 85 -> 138,0 */
	reached0[85] = 1;
	trans[0][85]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][86]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][87]	= settr(86,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][88]	= settr(87,2,90,659,659,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 0][90] = settr(89,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(89,0,89,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][89]	= settr(88,2,106,1,0,"(1)", 1, 2500, 0);
	T = trans[0][106] = settr(105,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(105,2,91,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(105,2,97,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(105,2,103,0,0,"IF", 1, 2500, 0);
	trans[0][91]	= settr(90,2,93,660,0,"((global_state.actuator1.cmd==2))", 1, 2500, 0);
	T = trans[ 0][93] = settr(92,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(92,0,92,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][92]	= settr(91,2,138,661,661,"Modemanager_actuatorTm1_channel!2", 1, 2500, 0); /* m: 94 -> 138,0 */
	reached0[94] = 1;
	trans[0][94]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][95]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][96]	= settr(95,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][107]	= settr(106,2,138,1,0,".(goto)", 1, 2500, 0);
	trans[0][97]	= settr(96,2,99,662,0,"((global_state.actuator1.cmd==1))", 1, 2500, 0);
	T = trans[ 0][99] = settr(98,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(98,0,98,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][98]	= settr(97,2,138,663,663,"Modemanager_actuatorTm1_channel!1", 1, 2500, 0); /* m: 100 -> 138,0 */
	reached0[100] = 1;
	trans[0][100]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][101]	= settr(0,0,0,0,0,"global_state.actuator1.state = 1",0,0,0);
	trans[0][102]	= settr(101,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][103]	= settr(102,2,104,2,0,"else", 1, 2500, 0);
	trans[0][104]	= settr(103,2,138,664,664,"transition_id = -(1)", 1, 2500, 0);
	trans[0][105]	= settr(104,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][108]	= settr(107,2,110,665,665,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 0][110] = settr(109,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(109,0,109,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][109]	= settr(108,2,120,1,0,"(1)", 1, 2500, 0);
	T = trans[0][120] = settr(119,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(119,2,111,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(119,2,117,0,0,"IF", 1, 2500, 0);
	trans[0][111]	= settr(110,2,113,666,0,"((global_state.actuator1.cmd==2))", 1, 2500, 0);
	T = trans[ 0][113] = settr(112,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(112,0,112,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][112]	= settr(111,2,138,667,667,"Modemanager_actuatorTm1_channel!2", 1, 2500, 0); /* m: 114 -> 138,0 */
	reached0[114] = 1;
	trans[0][114]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][115]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][116]	= settr(115,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][121]	= settr(120,2,138,1,0,".(goto)", 1, 2500, 0);
	trans[0][117]	= settr(116,2,118,2,0,"else", 1, 2500, 0);
	trans[0][118]	= settr(117,2,138,668,668,"transition_id = -(1)", 1, 2500, 0);
	trans[0][119]	= settr(118,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][122]	= settr(121,2,124,669,669,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 0][124] = settr(123,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(123,0,123,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][123]	= settr(122,2,134,1,0,"(1)", 1, 2500, 0);
	T = trans[0][134] = settr(133,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(133,2,125,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(133,2,131,0,0,"IF", 1, 2500, 0);
	trans[0][125]	= settr(124,2,127,670,0,"((global_state.actuator1.cmd==0))", 1, 2500, 0);
	T = trans[ 0][127] = settr(126,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(126,0,126,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][126]	= settr(125,2,138,671,671,"Modemanager_actuatorTm1_channel!0", 1, 2500, 0); /* m: 128 -> 138,0 */
	reached0[128] = 1;
	trans[0][128]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][129]	= settr(0,0,0,0,0,"global_state.actuator1.state = 0",0,0,0);
	trans[0][130]	= settr(129,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][135]	= settr(134,2,138,1,0,".(goto)", 1, 2500, 0);
	trans[0][131]	= settr(130,2,132,2,0,"else", 1, 2500, 0);
	trans[0][132]	= settr(131,2,138,672,672,"transition_id = -(1)", 1, 2500, 0);
	trans[0][133]	= settr(132,2,138,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][138]	= settr(137,2,139,1,0,"(1)", 1, 2500, 0);
	trans[0][141]	= settr(140,2,219,1,0,"break", 1, 2500, 0);
	trans[0][143]	= settr(142,2,147,673,0,"((global_state.actuator1.state==2))", 1, 2500, 0);
	T = trans[ 0][147] = settr(146,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(146,0,144,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][144]	= settr(143,2,146,674,674,"global_state.actuator1.cmd = Actuator1_actuatortc_signal_parameter", 1, 2500, 0);
	T = trans[ 0][146] = settr(145,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(145,0,145,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][145]	= settr(144,2,210,675,0,"assert((((global_state.actuator1.cmd==0)||(global_state.actuator1.cmd==1))||(global_state.actuator1.cmd==2)))", 1, 2500, 0);
	T = trans[ 0][210] = settr(209,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(209,0,148,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][148]	= settr(147,2,207,676,676,"transition_id = 0", 1, 2500, 0); /* m: 149 -> 0,207 */
	reached0[149] = 1;
	trans[0][149]	= settr(0,0,0,0,0,"transition_id = 3",0,0,0);
	trans[0][208]	= settr(207,2,207,1,0,".(goto)", 1, 2500, 0);
	T = trans[0][207] = settr(206,2,0,0,0,"DO", 1, 2500, 0);
	    T->nxt	= settr(206,2,204,0,0,"DO", 1, 2500, 0);
	T = trans[0][204] = settr(203,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(203,2,150,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(203,2,152,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(203,2,156,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(203,2,176,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(203,2,190,0,0,"IF", 1, 2500, 0);
	trans[0][150]	= settr(149,2,219,677,677,"((transition_id==-(1)))", 1, 2500, 0);
	trans[0][151]	= settr(150,2,219,1,0,"goto :b3", 1, 2500, 0);
	trans[0][205]	= settr(204,2,206,1,0,".(goto)", 1, 2500, 0);
	trans[0][152]	= settr(151,2,206,678,678,"((transition_id==0))", 1, 2500, 0); /* m: 153 -> 206,0 */
	reached0[153] = 1;
	trans[0][153]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][154]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][155]	= settr(154,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][156]	= settr(155,2,158,679,679,"((transition_id==1))", 1, 2500, 0);
	T = trans[ 0][158] = settr(157,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(157,0,157,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][157]	= settr(156,2,174,1,0,"(1)", 1, 2500, 0);
	T = trans[0][174] = settr(173,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(173,2,159,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(173,2,165,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(173,2,171,0,0,"IF", 1, 2500, 0);
	trans[0][159]	= settr(158,2,161,680,0,"((global_state.actuator1.cmd==2))", 1, 2500, 0);
	T = trans[ 0][161] = settr(160,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(160,0,160,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][160]	= settr(159,2,206,681,681,"Modemanager_actuatorTm1_channel!2", 1, 2500, 0); /* m: 162 -> 206,0 */
	reached0[162] = 1;
	trans[0][162]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][163]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][164]	= settr(163,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][175]	= settr(174,2,206,1,0,".(goto)", 1, 2500, 0);
	trans[0][165]	= settr(164,2,167,682,0,"((global_state.actuator1.cmd==1))", 1, 2500, 0);
	T = trans[ 0][167] = settr(166,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(166,0,166,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][166]	= settr(165,2,206,683,683,"Modemanager_actuatorTm1_channel!1", 1, 2500, 0); /* m: 168 -> 206,0 */
	reached0[168] = 1;
	trans[0][168]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][169]	= settr(0,0,0,0,0,"global_state.actuator1.state = 1",0,0,0);
	trans[0][170]	= settr(169,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][171]	= settr(170,2,172,2,0,"else", 1, 2500, 0);
	trans[0][172]	= settr(171,2,206,684,684,"transition_id = -(1)", 1, 2500, 0);
	trans[0][173]	= settr(172,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][176]	= settr(175,2,178,685,685,"((transition_id==2))", 1, 2500, 0);
	T = trans[ 0][178] = settr(177,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(177,0,177,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][177]	= settr(176,2,188,1,0,"(1)", 1, 2500, 0);
	T = trans[0][188] = settr(187,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(187,2,179,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(187,2,185,0,0,"IF", 1, 2500, 0);
	trans[0][179]	= settr(178,2,181,686,0,"((global_state.actuator1.cmd==2))", 1, 2500, 0);
	T = trans[ 0][181] = settr(180,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(180,0,180,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][180]	= settr(179,2,206,687,687,"Modemanager_actuatorTm1_channel!2", 1, 2500, 0); /* m: 182 -> 206,0 */
	reached0[182] = 1;
	trans[0][182]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][183]	= settr(0,0,0,0,0,"global_state.actuator1.state = 2",0,0,0);
	trans[0][184]	= settr(183,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][189]	= settr(188,2,206,1,0,".(goto)", 1, 2500, 0);
	trans[0][185]	= settr(184,2,186,2,0,"else", 1, 2500, 0);
	trans[0][186]	= settr(185,2,206,688,688,"transition_id = -(1)", 1, 2500, 0);
	trans[0][187]	= settr(186,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][190]	= settr(189,2,192,689,689,"((transition_id==3))", 1, 2500, 0);
	T = trans[ 0][192] = settr(191,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(191,0,191,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][191]	= settr(190,2,202,1,0,"(1)", 1, 2500, 0);
	T = trans[0][202] = settr(201,2,0,0,0,"IF", 1, 2500, 0);
	T = T->nxt	= settr(201,2,193,0,0,"IF", 1, 2500, 0);
	    T->nxt	= settr(201,2,199,0,0,"IF", 1, 2500, 0);
	trans[0][193]	= settr(192,2,195,690,0,"((global_state.actuator1.cmd==0))", 1, 2500, 0);
	T = trans[ 0][195] = settr(194,0,0,0,0,"sub-sequence", 1, 2500, 0);
	T->nxt	= settr(194,0,194,0,0,"sub-sequence", 1, 2500, 0);
	trans[0][194]	= settr(193,2,206,691,691,"Modemanager_actuatorTm1_channel!0", 1, 2500, 0); /* m: 196 -> 206,0 */
	reached0[196] = 1;
	trans[0][196]	= settr(0,0,0,0,0,"transition_id = -(1)",0,0,0);
	trans[0][197]	= settr(0,0,0,0,0,"global_state.actuator1.state = 0",0,0,0);
	trans[0][198]	= settr(197,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][203]	= settr(202,2,206,1,0,".(goto)", 1, 2500, 0);
	trans[0][199]	= settr(198,2,200,2,0,"else", 1, 2500, 0);
	trans[0][200]	= settr(199,2,206,692,692,"transition_id = -(1)", 1, 2500, 0);
	trans[0][201]	= settr(200,2,206,1,0,"goto continuous_signals", 1, 2500, 0);
	trans[0][206]	= settr(205,2,207,1,0,"(1)", 1, 2500, 0);
	trans[0][209]	= settr(208,2,219,1,0,"break", 1, 2500, 0);
	trans[0][211]	= settr(210,2,212,2,0,"else", 1, 2500, 0);
	trans[0][212]	= settr(211,2,219,1,0,"(1)", 1, 2500, 0);
	trans[0][216]	= settr(215,2,219,1,0,"goto Actuator1_actuatortc_loop", 1, 2500, 0);
	trans[0][220]	= settr(219,2,221,1,0,".(goto)", 1, 2500, 0);
	trans[0][217]	= settr(216,2,218,693,0,"(empty(Actuator1_actuatorTc_channel))", 1, 2500, 0);
	trans[0][218]	= settr(217,2,221,694,694,"(1)", 1, 2500, 0); /* m: 220 -> 221,0 */
	reached0[220] = 1;
	trans[0][221]	= settr(220,0,223,695,695,"Actuator1_lock!1", 1, 2500, 0);
	trans[0][225]	= settr(224,0,226,1,0,"break", 0, 2, 0);
	trans[0][226]	= settr(225,0,0,696,696,"-end-", 0, 3500, 0);
	/* np_ demon: */
	trans[_NP_] = (Trans **) emalloc(3*sizeof(Trans *));
	T = trans[_NP_][0] = settr(9997,0,1,_T5,0,"(np_)", 1,2,0);
	    T->nxt	  = settr(9998,0,0,_T2,0,"(1)",   0,2,0);
	T = trans[_NP_][1] = settr(9999,0,1,_T5,0,"(np_)", 1,2,0);
}

Trans *
settr(	int t_id, int a, int b, int c, int d,
	char *t, int g, int tpe0, int tpe1)
{	Trans *tmp = (Trans *) emalloc(sizeof(Trans));

	tmp->atom  = a&(6|32);	/* only (2|8|32) have meaning */
	if (!g) tmp->atom |= 8;	/* no global references */
	tmp->st    = b;
	tmp->tpe[0] = tpe0;
	tmp->tpe[1] = tpe1;
	tmp->tp    = t;
	tmp->t_id  = t_id;
	tmp->forw  = c;
	tmp->back  = d;
	return tmp;
}

Trans *
cpytr(Trans *a)
{	Trans *tmp = (Trans *) emalloc(sizeof(Trans));

	int i;
	tmp->atom  = a->atom;
	tmp->st    = a->st;
#ifdef HAS_UNLESS
	tmp->e_trans = a->e_trans;
	for (i = 0; i < HAS_UNLESS; i++)
		tmp->escp[i] = a->escp[i];
#endif
	tmp->tpe[0] = a->tpe[0];
	tmp->tpe[1] = a->tpe[1];
	for (i = 0; i < 6; i++)
	{	tmp->qu[i] = a->qu[i];
		tmp->ty[i] = a->ty[i];
	}
	tmp->tp    = (char *) emalloc(strlen(a->tp)+1);
	strcpy(tmp->tp, a->tp);
	tmp->t_id  = a->t_id;
	tmp->forw  = a->forw;
	tmp->back  = a->back;
	return tmp;
}

#ifndef NOREDUCE
int
srinc_set(int n)
{	if (n <= 2) return LOCAL;
	if (n <= 2+  DELTA) return Q_FULL_F; /* 's' or nfull  */
	if (n <= 2+2*DELTA) return Q_EMPT_F; /* 'r' or nempty */
	if (n <= 2+3*DELTA) return Q_EMPT_T; /* empty */
	if (n <= 2+4*DELTA) return Q_FULL_T; /* full  */
	if (n ==   5*DELTA) return GLOBAL;
	if (n ==   6*DELTA) return TIMEOUT_F;
	if (n ==   7*DELTA) return ALPHA_F;
	Uerror("cannot happen srinc_class");
	return BAD;
}
int
srunc(int n, int m)
{	switch(m) {
	case Q_FULL_F: return n-2;
	case Q_EMPT_F: return n-2-DELTA;
	case Q_EMPT_T: return n-2-2*DELTA;
	case Q_FULL_T: return n-2-3*DELTA;
	case ALPHA_F:
	case TIMEOUT_F: return 257; /* non-zero, and > MAXQ */
	}
	Uerror("cannot happen srunc");
	return 0;
}
#endif
int cnt;
#ifdef HAS_UNLESS
int
isthere(Trans *a, int b)
{	Trans *t;
	for (t = a; t; t = t->nxt)
		if (t->t_id == b)
			return 1;
	return 0;
}
#endif
#ifndef NOREDUCE
int
mark_safety(Trans *t) /* for conditional safety */
{	int g = 0, i, j, k;

	if (!t) return 0;
	if (t->qu[0])
		return (t->qu[1])?2:1;	/* marked */

	for (i = 0; i < 2; i++)
	{	j = srinc_set(t->tpe[i]);
		if (j >= GLOBAL && j != ALPHA_F)
			return -1;
		if (j != LOCAL)
		{	k = srunc(t->tpe[i], j);
			if (g == 0
			||  t->qu[0] != k
			||  t->ty[0] != j)
			{	t->qu[g] = k;
				t->ty[g] = j;
				g++;
	}	}	}
	return g;
}
#endif
void
retrans(int n, int m, int is, short srcln[], uchar reach[], uchar lpstate[])
	/* process n, with m states, is=initial state */
{	Trans *T0, *T1, *T2, *T3;
	Trans *T4, *T5; /* t_reverse or has_unless */
	int i;
#if defined(HAS_UNLESS) || !defined(NOREDUCE)
	int k;
#endif
#ifndef NOREDUCE
	int g, h, j, aa;
#endif
#ifdef HAS_UNLESS
	int p;
#endif
	if (state_tables >= 4)
	{	printf("STEP 1 %s\n", 
			procname[n]);
		for (i = 1; i < m; i++)
		for (T0 = trans[n][i]; T0; T0 = T0->nxt)
			crack(n, i, T0, srcln);
		return;
	}
	do {
		for (i = 1, cnt = 0; i < m; i++)
		{	T2 = trans[n][i];
			T1 = T2?T2->nxt:(Trans *)0;
/* prescan: */		for (T0 = T1; T0; T0 = T0->nxt)
/* choice in choice */	{	if (T0->st && trans[n][T0->st]
				&&  trans[n][T0->st]->nxt)
					break;
			}
#if 0
		if (T0)
		printf("\tstate %d / %d: choice in choice\n",
		i, T0->st);
#endif
			if (T0)
			for (T0 = T1; T0; T0 = T0->nxt)
			{	T3 = trans[n][T0->st];
				if (!T3->nxt)
				{	T2->nxt = cpytr(T0);
					T2 = T2->nxt;
					imed(T2, T0->st, n, i);
					continue;
				}
				do {	T3 = T3->nxt;
					T2->nxt = cpytr(T3);
					T2 = T2->nxt;
					imed(T2, T0->st, n, i);
				} while (T3->nxt);
				cnt++;
			}
		}
	} while (cnt);
	if (state_tables >= 3)
	{	printf("STEP 2 %s\n", 
			procname[n]);
		for (i = 1; i < m; i++)
		for (T0 = trans[n][i]; T0; T0 = T0->nxt)
			crack(n, i, T0, srcln);
		return;
	}
	for (i = 1; i < m; i++)
	{	if (trans[n][i] && trans[n][i]->nxt) /* optimize */
		{	T1 = trans[n][i]->nxt;
#if 0
			printf("\t\tpull %d (%d) to %d\n",
			T1->st, T1->forw, i);
#endif
			srcln[i] = srcln[T1->st];	/* Oyvind Teig, 5.2.0 */

			if (!trans[n][T1->st]) continue;
			T0 = cpytr(trans[n][T1->st]);
			trans[n][i] = T0;
			reach[T1->st] = 1;
			imed(T0, T1->st, n, i);
			for (T1 = T1->nxt; T1; T1 = T1->nxt)
			{
#if 0
			printf("\t\tpull %d (%d) to %d\n",
				T1->st, T1->forw, i);
#endif
		/*		srcln[i] = srcln[T1->st];  gh: not useful */
				if (!trans[n][T1->st]) continue;
				T0->nxt = cpytr(trans[n][T1->st]);
				T0 = T0->nxt;
				reach[T1->st] = 1;
				imed(T0, T1->st, n, i);
	}	}	}
	if (state_tables >= 2)
	{	printf("STEP 3 %s\n", 
			procname[n]);
		for (i = 1; i < m; i++)
		for (T0 = trans[n][i]; T0; T0 = T0->nxt)
			crack(n, i, T0, srcln);
		return;
	}
#ifdef HAS_UNLESS
	for (i = 1; i < m; i++)
	{	if (!trans[n][i]) continue;
		/* check for each state i if an
		 * escape to some state p is defined
		 * if so, copy and mark p's transitions
		 * and prepend them to the transition-
		 * list of state i
		 */
	 if (!like_java) /* the default */
	 {	for (T0 = trans[n][i]; T0; T0 = T0->nxt)
		for (k = HAS_UNLESS-1; k >= 0; k--)
		{	if (p = T0->escp[k])
			for (T1 = trans[n][p]; T1; T1 = T1->nxt)
			{	if (isthere(trans[n][i], T1->t_id))
					continue;
				T2 = cpytr(T1);
				T2->e_trans = p;
				T2->nxt = trans[n][i];
				trans[n][i] = T2;
		}	}
	 } else /* outermost unless checked first */
	 {	T4 = T3 = (Trans *) 0;
		for (T0 = trans[n][i]; T0; T0 = T0->nxt)
		for (k = HAS_UNLESS-1; k >= 0; k--)
		{	if (p = T0->escp[k])
			for (T1 = trans[n][p]; T1; T1 = T1->nxt)
			{	if (isthere(trans[n][i], T1->t_id))
					continue;
				T2 = cpytr(T1);
				T2->nxt = (Trans *) 0;
				T2->e_trans = p;
				if (T3)	T3->nxt = T2;
				else	T4 = T2;
				T3 = T2;
		}	}
		if (T4)
		{	T3->nxt = trans[n][i];
			trans[n][i] = T4;
		}
	 }
	}
#endif
#ifndef NOREDUCE
	for (i = 1; i < m; i++)
	{	if (a_cycles)
		{ /* moves through these states are visible */
	#if PROG_LAB>0 && defined(HAS_NP)
			if (progstate[n][i])
				goto degrade;
			for (T1 = trans[n][i]; T1; T1 = T1->nxt)
				if (progstate[n][T1->st])
					goto degrade;
	#endif
			if (accpstate[n][i] || visstate[n][i])
				goto degrade;
			for (T1 = trans[n][i]; T1; T1 = T1->nxt)
				if (accpstate[n][T1->st])
					goto degrade;
		}
		T1 = trans[n][i];
		if (!T1) continue;
		g = mark_safety(T1);	/* V3.3.1 */
		if (g < 0) goto degrade; /* global */
		/* check if mixing of guards preserves reduction */
		if (T1->nxt)
		{	k = 0;
			for (T0 = T1; T0; T0 = T0->nxt)
			{	if (!(T0->atom&8))
					goto degrade;
				for (aa = 0; aa < 2; aa++)
				{	j = srinc_set(T0->tpe[aa]);
					if (j >= GLOBAL && j != ALPHA_F)
						goto degrade;
					if (T0->tpe[aa]
					&&  T0->tpe[aa]
					!=  T1->tpe[0])
						k = 1;
			}	}
			/* g = 0;	V3.3.1 */
			if (k)	/* non-uniform selection */
			for (T0 = T1; T0; T0 = T0->nxt)
			for (aa = 0; aa < 2; aa++)
			{	j = srinc_set(T0->tpe[aa]);
				if (j != LOCAL)
				{	k = srunc(T0->tpe[aa], j);
					for (h = 0; h < 6; h++)
						if (T1->qu[h] == k
						&&  T1->ty[h] == j)
							break;
					if (h >= 6)
					{	T1->qu[g%6] = k;
						T1->ty[g%6] = j;
						g++;
			}	}	}
			if (g > 6)
			{	T1->qu[0] = 0;	/* turn it off */
				printf("pan: warning, line %d, ",
					srcln[i]);
			 	printf("too many stmnt types (%d)",
					g);
			  	printf(" in selection\n");
			  goto degrade;
			}
		}
		/* mark all options global if >=1 is global */
		for (T1 = trans[n][i]; T1; T1 = T1->nxt)
			if (!(T1->atom&8)) break;
		if (T1)
degrade:	for (T1 = trans[n][i]; T1; T1 = T1->nxt)
			T1->atom &= ~8;	/* mark as unsafe */
		/* can only mix 'r's or 's's if on same chan */
		/* and not mixed with other local operations */
		T1 = trans[n][i];
		if (!T1 || T1->qu[0]) continue;
		j = T1->tpe[0];
		if (T1->nxt && T1->atom&8)
		{ if (j == 5*DELTA)
		  {	printf("warning: line %d ", srcln[i]);
			printf("mixed condition ");
			printf("(defeats reduction)\n");
			goto degrade;
		  }
		  for (T0 = T1; T0; T0 = T0->nxt)
		  for (aa = 0; aa < 2; aa++)
		  if  (T0->tpe[aa] && T0->tpe[aa] != j)
		  {	printf("warning: line %d ", srcln[i]);
			printf("[%d-%d] mixed %stion ",
				T0->tpe[aa], j, 
				(j==5*DELTA)?"condi":"selec");
			printf("(defeats reduction)\n");
			printf("	'%s' <-> '%s'\n",
				T1->tp, T0->tp);
			goto degrade;
		} }
	}
#endif
	for (i = 1; i < m; i++)
	{	T2 = trans[n][i];
		if (!T2
		||  T2->nxt
		||  strncmp(T2->tp, ".(goto)", 7)
		||  !stopstate[n][i])
			continue;
		stopstate[n][T2->st] = 1;
	}
	if (state_tables && !verbose)
	{	if (dodot)
		{	char buf[256], *q = buf, *p = procname[n];
			while (*p != '\0')
			{	if (*p != ':')
				{	*q++ = *p;
				}
				p++;
			}
			*q = '\0';
			printf("digraph ");
			switch (Btypes[n]) {
			case I_PROC:  printf("init {\n"); break;
			case N_CLAIM: printf("claim_%s {\n", buf); break;
			case E_TRACE: printf("notrace {\n"); break;
			case N_TRACE: printf("trace {\n"); break;
			default:      printf("p_%s {\n", buf); break;
			}
			printf("size=\"8,10\";\n");
			printf("  GT [shape=box,style=dotted,label=\"%s\"];\n", buf);
			printf("  GT -> S%d;\n", is);
		} else
		{	switch (Btypes[n]) {
			case I_PROC:  printf("init\n"); break;
			case N_CLAIM: printf("claim %s\n", procname[n]); break;
			case E_TRACE: printf("notrace assertion\n"); break;
			case N_TRACE: printf("trace assertion\n"); break;
			default:      printf("proctype %s\n", procname[n]); break;
		}	}
		for (i = 1; i < m; i++)
		{	reach[i] = 1;
		}
		tagtable(n, m, is, srcln, reach);
		if (dodot) printf("}\n");
	} else
	for (i = 1; i < m; i++)
	{	int nrelse;
		if (Btypes[n] != N_CLAIM)
		{	for (T0 = trans[n][i]; T0; T0 = T0->nxt)
			{	if (T0->st == i
				&& strcmp(T0->tp, "(1)") == 0)
				{	printf("error: proctype '%s' ",
						procname[n]);
		  			printf("line %d, state %d: has un",
						srcln[i], i);
					printf("conditional self-loop\n");
					pan_exit(1);
		}	}	}
		nrelse = 0;
		for (T0 = trans[n][i]; T0; T0 = T0->nxt)
		{	if (strcmp(T0->tp, "else") == 0)
				nrelse++;
		}
		if (nrelse > 1)
		{	printf("error: proctype '%s' state",
				procname[n]);
		  	printf(" %d, inherits %d", i, nrelse);
		  	printf(" 'else' stmnts\n");
			pan_exit(1);
	}	}
#if !defined(LOOPSTATE) && !defined(BFS_PAR)
	if (state_tables)
#endif
	do_dfs(n, m, is, srcln, reach, lpstate);

	if (!t_reverse)
	{	return;
	}
	/* process n, with m states, is=initial state -- reverse list */
	if (!state_tables && Btypes[n] != N_CLAIM)
	{	for (i = 1; i < m; i++)
		{	Trans *Tx = (Trans *) 0; /* list of escapes */
			Trans *Ty = (Trans *) 0; /* its tail element */
			T1 = (Trans *) 0; /* reversed list */
			T2 = (Trans *) 0; /* its tail */
			T3 = (Trans *) 0; /* remembers possible 'else' */

			/* find unless-escapes, they should go first */
			T4 = T5 = T0 = trans[n][i];
	#ifdef HAS_UNLESS
			while (T4 && T4->e_trans) /* escapes are first in orig list */
			{	T5 = T4;	  /* remember predecessor */
				T4 = T4->nxt;
			}
	#endif
			/* T4 points to first non-escape, T5 to its parent, T0 to original list */
			if (T4 != T0)		 /* there was at least one escape */
			{	T3 = T5->nxt;		 /* start of non-escapes */
				T5->nxt = (Trans *) 0;	 /* separate */
				Tx = T0;		 /* start of the escapes */
				Ty = T5;		 /* its tail */
				T0 = T3;		 /* the rest, to be reversed */
			}
			/* T0 points to first non-escape, Tx to the list of escapes, Ty to its tail */

			/* first tail-add non-escape transitions, reversed */
			T3 = (Trans *) 0;
			for (T5 = T0; T5; T5 = T4)
			{	T4 = T5->nxt;
	#ifdef HAS_UNLESS
				if (T5->e_trans)
				{	printf("error: cannot happen!\n");
					continue;
				}
	#endif
				if (strcmp(T5->tp, "else") == 0)
				{	T3 = T5;
					T5->nxt = (Trans *) 0;
				} else
				{	T5->nxt = T1;
					if (!T1) { T2 = T5; }
					T1 = T5;
			}	}
			/* T3 points to a possible else, which is removed from the list */
			/* T1 points to the reversed list so far (without escapes) */
			/* T2 points to the tail element -- where the else should go */
			if (T2 && T3)
			{	T2->nxt = T3;	/* add else */
			} else
			{	if (T3) /* there was an else, but there's no tail */
				{	if (!T1)	/* and no reversed list */
					{	T1 = T3; /* odd, but possible */
					} else		/* even stranger */
					{	T1->nxt = T3;
			}	}	}

			/* add in the escapes, to that they appear at the front */
			if (Tx && Ty) { Ty->nxt = T1; T1 = Tx; }

			trans[n][i] = T1;
			/* reversed, with escapes first and else last */
	}	}
	if (state_tables && verbose)
	{	printf("FINAL proctype %s\n", 
			procname[n]);
		for (i = 1; i < m; i++)
		for (T0 = trans[n][i]; T0; T0 = T0->nxt)
			crack(n, i, T0, srcln);
	}
}
void
imed(Trans *T, int v, int n, int j)	/* set intermediate state */
{	progstate[n][T->st] |= progstate[n][v];
	accpstate[n][T->st] |= accpstate[n][v];
	stopstate[n][T->st] |= stopstate[n][v];
	mapstate[n][j] = T->st;
}
void
tagtable(int n, int m, int is, short srcln[], uchar reach[])
{	Trans *z;

	if (is >= m || !trans[n][is]
	||  is <= 0 || reach[is] == 0)
		return;
	reach[is] = 0;
	if (state_tables)
	for (z = trans[n][is]; z; z = z->nxt)
	{	if (dodot)
			dot_crack(n, is, z);
		else
			crack(n, is, z, srcln);
	}

	for (z = trans[n][is]; z; z = z->nxt)
	{
#ifdef HAS_UNLESS
		int i, j;
#endif
		tagtable(n, m, z->st, srcln, reach);
#ifdef HAS_UNLESS
		for (i = 0; i < HAS_UNLESS; i++)
		{	j = trans[n][is]->escp[i];
			if (!j) break;
			tagtable(n, m, j, srcln, reach);
		}
#endif
	}
}

extern Trans *t_id_lkup[];

void
dfs_table(int n, int m, int is, short srcln[], uchar reach[], uchar lpstate[])
{	Trans *z;

	if (is >= m || is <= 0 || !trans[n][is])
		return;
	if ((reach[is] & (4|8|16)) != 0)
	{	if ((reach[is] & (8|16)) == 16)	/* on stack, not yet recorded */
		{	lpstate[is] = 1;
			reach[is] |= 8; /* recorded */
			if (state_tables && verbose)
			{	printf("state %d line %d is a loopstate\n", is, srcln[is]);
		}	}
		return;
	}
	reach[is] |= (4|16);	/* visited | onstack */
	for (z = trans[n][is]; z; z = z->nxt)
	{	t_id_lkup[z->t_id] = z;
#ifdef HAS_UNLESS
		int i, j;
#endif
		dfs_table(n, m, z->st, srcln, reach, lpstate);
#ifdef HAS_UNLESS
		for (i = 0; i < HAS_UNLESS; i++)
		{	j = trans[n][is]->escp[i];
			if (!j) break;
			dfs_table(n, m, j, srcln, reach, lpstate);
		}
#endif
	}
	reach[is] &= ~16; /* no longer on stack */
}
void
do_dfs(int n, int m, int is, short srcln[], uchar reach[], uchar lpstate[])
{	int i;
	dfs_table(n, m, is, srcln, reach, lpstate);
	for (i = 0; i < m; i++)
		reach[i] &= ~(4|8|16);
}
void
crack(int n, int j, Trans *z, short srcln[])
{	int i;

	if (!z) return;
	printf("	state %3d -(tr %3d)-> state %3d  ",
		j, z->forw, z->st);
	printf("[id %3d tp %3d", z->t_id, z->tpe[0]);
	if (z->tpe[1]) printf(",%d", z->tpe[1]);
#ifdef HAS_UNLESS
	if (z->e_trans)
		printf(" org %3d", z->e_trans);
	else if (state_tables >= 2)
	for (i = 0; i < HAS_UNLESS; i++)
	{	if (!z->escp[i]) break;
		printf(" esc %d", z->escp[i]);
	}
#endif
	printf("]");
	printf(" [%s%s%s%s%s] %s:%d => ",
		z->atom&6?"A":z->atom&32?"D":"-",
		accpstate[n][j]?"a" :"-",
		stopstate[n][j]?"e" : "-",
		progstate[n][j]?"p" : "-",
		z->atom & 8 ?"L":"G",
		PanSource, srcln[j]);
	for (i = 0; z->tp[i]; i++)
		if (z->tp[i] == '\n')
			printf("\\n");
		else
			putchar(z->tp[i]);
	if (verbose && z->qu[0])
	{	printf("\t[");
		for (i = 0; i < 6; i++)
			if (z->qu[i])
				printf("(%d,%d)",
				z->qu[i], z->ty[i]);
		printf("]");
	}
	printf("\n");
	fflush(stdout);
}
/* spin -a m.pml; cc -o pan pan.c; ./pan -D | dot -Tps > foo.ps; ps2pdf foo.ps */
void
dot_crack(int n, int j, Trans *z)
{	int i;

	if (!z) return;
	printf("	S%d -> S%d  [color=black", j, z->st);

	if (z->atom&6) printf(",style=dashed");
	else if (z->atom&32) printf(",style=dotted");
	else if (z->atom&8) printf(",style=solid");
	else printf(",style=bold");

	printf(",label=\"");
	for (i = 0; z->tp[i]; i++)
	{	if (z->tp[i] == '\\'
		&&  z->tp[i+1] == 'n')
		{	i++; printf(" ");
		} else
		{	putchar(z->tp[i]);
	}	}
	printf("\"];\n");
	if (accpstate[n][j]) printf("  S%d [color=red,style=bold];\n", j);
	else if (progstate[n][j]) printf("  S%d [color=green,style=bold];\n", j);
	if (stopstate[n][j]) printf("  S%d [color=blue,style=bold,shape=box];\n", j);
}

#ifdef VAR_RANGES
#define BYTESIZE	32	/* 2^8 : 2^3 = 256:8 = 32 */

typedef struct Vr_Ptr {
	char	*nm;
	uchar	vals[BYTESIZE];
	struct Vr_Ptr *nxt;
} Vr_Ptr;
Vr_Ptr *ranges = (Vr_Ptr *) 0;

void
logval(char *s, int v)
{	Vr_Ptr *tmp;

	if (v<0 || v > 255) return;
	for (tmp = ranges; tmp; tmp = tmp->nxt)
		if (!strcmp(tmp->nm, s))
			goto found;
	tmp = (Vr_Ptr *) emalloc(sizeof(Vr_Ptr));
	tmp->nxt = ranges;
	ranges = tmp;
	tmp->nm = s;
found:
	tmp->vals[(v)/8] |= 1<<((v)%8);
}

void
dumpval(uchar X[], int range)
{	int w, x, i, j = -1;

	for (w = i = 0; w < range; w++)
	for (x = 0; x < 8; x++, i++)
	{
from:		if ((X[w] & (1<<x)))
		{	printf("%d", i);
			j = i;
			goto upto;
	}	}
	return;
	for (w = 0; w < range; w++)
	for (x = 0; x < 8; x++, i++)
	{
upto:		if (!(X[w] & (1<<x)))
		{	if (i-1 == j)
				printf(", ");
			else
				printf("-%d, ", i-1);
			goto from;
	}	}
	if (j >= 0 && j != 255)
		printf("-255");
}

void
dumpranges(void)
{	Vr_Ptr *tmp;
	printf("\nValues assigned within ");
	printf("interval [0..255]:\n");
	for (tmp = ranges; tmp; tmp = tmp->nxt)
	{	printf("\t%s\t: ", tmp->nm);
		dumpval(tmp->vals, BYTESIZE);
		printf("\n");
	}
}
#endif
