/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "new_model.h"
#include "string.h"
#include "math.h"



struct _SE3_Model* Create_SE3_Model()
{
    struct _SE3_Model* M = (struct _SE3_Model*) calloc(1,sizeof(struct _SE3_Model));

    M->NT.Entries[0].x=0.1556;
M->NT.Entries[1].x=0.2751;
M->NT.Entries[2].x=-0.0407;
M->NT.Entries[3].x=0.0671;
M->NT.Entries[4].x=0.3038;
M->NT.Entries[5].x=0.373;
M->NT.Entries[6].x=0.4603;
M->NT.Entries[7].x=0.5678;
M->NT.Entries[8].x=0.099;
M->NT.Entries[9].x=-0.1114;
M->NT.Entries[10].x=0.117;
M->NT.Entries[11].x=-0.1242;
M->NT.Entries[12].x=0.6517;
M->NT.Entries[13].x=0.5645;
M->NT.Entries[14].x=0.7929;
M->NT.Entries[15].x=0.7178;
M->NT.Entries[16].x=0.5036;
M->NT.Entries[17].x=0.3792;
M->NT.Entries[18].x=0.3792;
M->NT.Entries[19].x=0.2171;
M->NT.Entries[20].x=0.6517;
M->NT.Entries[21].x=0.7929;
M->NT.Entries[22].x=0.5645;
M->NT.Entries[23].x=0.7178;
M->NT.Entries[24].x=-0.4074;
M->NT.Entries[25].x=-0.2092;
M->NT.Entries[26].x=-0.4044;
M->NT.Entries[27].x=-0.1756;
M->NT.Entries[28].x=-0.464;
M->NT.Entries[29].x=-0.5624;
M->NT.Entries[30].x=-0.28;
M->NT.Entries[31].x=-0.3669;
M->NT.Entries[32].x=-0.5905;
M->NT.Entries[33].x=-0.6334;
M->NT.Entries[34].x=-0.7206;
M->NT.Entries[35].x=-0.7946;
M->NT.Entries[36].x=-0.4074;
M->NT.Entries[37].x=-0.4044;
M->NT.Entries[38].x=-0.2092;
M->NT.Entries[39].x=-0.1756;
M->NT.Entries[40].x=-0.5905;
M->NT.Entries[41].x=-0.7206;
M->NT.Entries[42].x=-0.6334;
M->NT.Entries[43].x=-0.7946;
M->NT.Entries[44].x=-0.464;
M->NT.Entries[45].x=-0.28;
M->NT.Entries[46].x=-0.5624;
M->NT.Entries[47].x=-0.3669;
M->NT.Entries[48].x=0.1556;
M->NT.Entries[49].x=-0.0407;
M->NT.Entries[50].x=0.2751;
M->NT.Entries[51].x=0.0671;
M->NT.Entries[52].x=0.099;
M->NT.Entries[53].x=0.117;
M->NT.Entries[54].x=-0.1114;
M->NT.Entries[55].x=-0.1242;
M->NT.Entries[56].x=0.3038;
M->NT.Entries[57].x=0.4603;
M->NT.Entries[58].x=0.373;
M->NT.Entries[59].x=0.5678;
M->NT.Entries[60].x=0.963;
M->NT.Entries[61].x=0.8861;
M->NT.Entries[62].x=0.9401;
M->NT.Entries[63].x=0.852;
M->NT.Entries[64].x=0.963;
M->NT.Entries[65].x=0.9401;
M->NT.Entries[66].x=0.8861;
M->NT.Entries[67].x=0.852;
M->NT.Entries[68].x=0.9979;
M->NT.Entries[69].x=0.9838;
M->NT.Entries[70].x=0.9838;
M->NT.Entries[71].x=0.9702;
M->NT.Entries[72].x=0.1952;
M->NT.Entries[73].x=0.1458;
M->NT.Entries[74].x=0.0046;
M->NT.Entries[75].x=-0.0827;
M->NT.Entries[76].x=0.3999;
M->NT.Entries[77].x=0.5764;
M->NT.Entries[78].x=0.4018;
M->NT.Entries[79].x=0.6093;
M->NT.Entries[80].x=0.3084;
M->NT.Entries[81].x=0.1461;
M->NT.Entries[82].x=0.4619;
M->NT.Entries[83].x=0.2998;
M->NT.Entries[84].x=-0.8423;
M->NT.Entries[85].x=-0.796;
M->NT.Entries[86].x=-0.9372;
M->NT.Entries[87].x=-0.9031;
M->NT.Entries[88].x=-0.7158;
M->NT.Entries[89].x=-0.5838;
M->NT.Entries[90].x=-0.6378;
M->NT.Entries[91].x=-0.4754;
M->NT.Entries[92].x=-0.8074;
M->NT.Entries[93].x=-0.8935;
M->NT.Entries[94].x=-0.6983;
M->NT.Entries[95].x=-0.7849;
M->NT.Entries[96].x=-0.7158;
M->NT.Entries[97].x=-0.6378;
M->NT.Entries[98].x=-0.5838;
M->NT.Entries[99].x=-0.4754;
M->NT.Entries[100].x=-0.8423;
M->NT.Entries[101].x=-0.9372;
M->NT.Entries[102].x=-0.796;
M->NT.Entries[103].x=-0.9031;
M->NT.Entries[104].x=-0.8074;
M->NT.Entries[105].x=-0.6983;
M->NT.Entries[106].x=-0.8935;
M->NT.Entries[107].x=-0.7849;
M->NT.Entries[108].x=0.3999;
M->NT.Entries[109].x=0.4018;
M->NT.Entries[110].x=0.5764;
M->NT.Entries[111].x=0.6093;
M->NT.Entries[112].x=0.1952;
M->NT.Entries[113].x=0.0046;
M->NT.Entries[114].x=0.1458;
M->NT.Entries[115].x=-0.0827;
M->NT.Entries[116].x=0.3084;
M->NT.Entries[117].x=0.4619;
M->NT.Entries[118].x=0.1461;
M->NT.Entries[119].x=0.2998;
M->NT.Entries[120].x=0.8074;
M->NT.Entries[121].x=0.8935;
M->NT.Entries[122].x=0.6983;
M->NT.Entries[123].x=0.7849;
M->NT.Entries[124].x=0.8423;
M->NT.Entries[125].x=0.796;
M->NT.Entries[126].x=0.9372;
M->NT.Entries[127].x=0.9031;
M->NT.Entries[128].x=0.7158;
M->NT.Entries[129].x=0.5838;
M->NT.Entries[130].x=0.6378;
M->NT.Entries[131].x=0.4754;
M->NT.Entries[132].x=-0.3084;
M->NT.Entries[133].x=-0.1461;
M->NT.Entries[134].x=-0.4619;
M->NT.Entries[135].x=-0.2998;
M->NT.Entries[136].x=-0.1952;
M->NT.Entries[137].x=-0.1458;
M->NT.Entries[138].x=-0.0046;
M->NT.Entries[139].x=0.0827;
M->NT.Entries[140].x=-0.3999;
M->NT.Entries[141].x=-0.5764;
M->NT.Entries[142].x=-0.4018;
M->NT.Entries[143].x=-0.6093;
M->NT.Entries[144].x=-0.9979;
M->NT.Entries[145].x=-0.9838;
M->NT.Entries[146].x=-0.9838;
M->NT.Entries[147].x=-0.9702;
M->NT.Entries[148].x=-0.963;
M->NT.Entries[149].x=-0.8861;
M->NT.Entries[150].x=-0.9401;
M->NT.Entries[151].x=-0.852;
M->NT.Entries[152].x=-0.963;
M->NT.Entries[153].x=-0.9401;
M->NT.Entries[154].x=-0.8861;
M->NT.Entries[155].x=-0.852;
M->NT.Entries[156].x=-0.3084;
M->NT.Entries[157].x=-0.4619;
M->NT.Entries[158].x=-0.1461;
M->NT.Entries[159].x=-0.2998;
M->NT.Entries[160].x=-0.3999;
M->NT.Entries[161].x=-0.4018;
M->NT.Entries[162].x=-0.5764;
M->NT.Entries[163].x=-0.6093;
M->NT.Entries[164].x=-0.1952;
M->NT.Entries[165].x=-0.0046;
M->NT.Entries[166].x=-0.1458;
M->NT.Entries[167].x=0.0827;
M->NT.Entries[168].x=0.8074;
M->NT.Entries[169].x=0.6983;
M->NT.Entries[170].x=0.8935;
M->NT.Entries[171].x=0.7849;
M->NT.Entries[172].x=0.7158;
M->NT.Entries[173].x=0.6378;
M->NT.Entries[174].x=0.5838;
M->NT.Entries[175].x=0.4754;
M->NT.Entries[176].x=0.8423;
M->NT.Entries[177].x=0.9372;
M->NT.Entries[178].x=0.796;
M->NT.Entries[179].x=0.9031;
M->NT.Entries[180].x=0.464;
M->NT.Entries[181].x=0.5624;
M->NT.Entries[182].x=0.28;
M->NT.Entries[183].x=0.3669;
M->NT.Entries[184].x=0.5905;
M->NT.Entries[185].x=0.6334;
M->NT.Entries[186].x=0.7206;
M->NT.Entries[187].x=0.7946;
M->NT.Entries[188].x=0.4074;
M->NT.Entries[189].x=0.2092;
M->NT.Entries[190].x=0.4044;
M->NT.Entries[191].x=0.1756;
M->NT.Entries[192].x=-0.3038;
M->NT.Entries[193].x=-0.373;
M->NT.Entries[194].x=-0.4603;
M->NT.Entries[195].x=-0.5678;
M->NT.Entries[196].x=-0.099;
M->NT.Entries[197].x=0.1114;
M->NT.Entries[198].x=-0.117;
M->NT.Entries[199].x=0.1242;
M->NT.Entries[200].x=-0.1556;
M->NT.Entries[201].x=-0.2751;
M->NT.Entries[202].x=0.0407;
M->NT.Entries[203].x=-0.0671;
M->NT.Entries[204].x=-0.6517;
M->NT.Entries[205].x=-0.7929;
M->NT.Entries[206].x=-0.5645;
M->NT.Entries[207].x=-0.7178;
M->NT.Entries[208].x=-0.6517;
M->NT.Entries[209].x=-0.5645;
M->NT.Entries[210].x=-0.7929;
M->NT.Entries[211].x=-0.7178;
M->NT.Entries[212].x=-0.5036;
M->NT.Entries[213].x=-0.3792;
M->NT.Entries[214].x=-0.3792;
M->NT.Entries[215].x=-0.2171;
M->NT.Entries[216].x=-0.099;
M->NT.Entries[217].x=-0.117;
M->NT.Entries[218].x=0.1114;
M->NT.Entries[219].x=0.1242;
M->NT.Entries[220].x=-0.3038;
M->NT.Entries[221].x=-0.4603;
M->NT.Entries[222].x=-0.373;
M->NT.Entries[223].x=-0.5678;
M->NT.Entries[224].x=-0.1556;
M->NT.Entries[225].x=0.0407;
M->NT.Entries[226].x=-0.2751;
M->NT.Entries[227].x=-0.0671;
M->NT.Entries[228].x=0.5905;
M->NT.Entries[229].x=0.7206;
M->NT.Entries[230].x=0.6334;
M->NT.Entries[231].x=0.7946;
M->NT.Entries[232].x=0.464;
M->NT.Entries[233].x=0.28;
M->NT.Entries[234].x=0.5624;
M->NT.Entries[235].x=0.3669;
M->NT.Entries[236].x=0.4074;
M->NT.Entries[237].x=0.4044;
M->NT.Entries[238].x=0.2092;
M->NT.Entries[239].x=0.1756;

M->NT.Entries[0].y=-0.8639;
M->NT.Entries[1].y=-0.9103;
M->NT.Entries[2].y=-0.9103;
M->NT.Entries[3].y=-0.9762;
M->NT.Entries[4].y=-0.7508;
M->NT.Entries[5].y=-0.5942;
M->NT.Entries[6].y=-0.7688;
M->NT.Entries[7].y=-0.5936;
M->NT.Entries[8].y=-0.7508;
M->NT.Entries[9].y=-0.7688;
M->NT.Entries[10].y=-0.5942;
M->NT.Entries[11].y=-0.5936;
M->NT.Entries[12].y=-0.7508;
M->NT.Entries[13].y=-0.7688;
M->NT.Entries[14].y=-0.5942;
M->NT.Entries[15].y=-0.5936;
M->NT.Entries[16].y=-0.8639;
M->NT.Entries[17].y=-0.9103;
M->NT.Entries[18].y=-0.9103;
M->NT.Entries[19].y=-0.9762;
M->NT.Entries[20].y=-0.7508;
M->NT.Entries[21].y=-0.5942;
M->NT.Entries[22].y=-0.7688;
M->NT.Entries[23].y=-0.5936;
M->NT.Entries[24].y=-0.8639;
M->NT.Entries[25].y=-0.9103;
M->NT.Entries[26].y=-0.9103;
M->NT.Entries[27].y=-0.9762;
M->NT.Entries[28].y=-0.7508;
M->NT.Entries[29].y=-0.5942;
M->NT.Entries[30].y=-0.7688;
M->NT.Entries[31].y=-0.5936;
M->NT.Entries[32].y=-0.7508;
M->NT.Entries[33].y=-0.7688;
M->NT.Entries[34].y=-0.5942;
M->NT.Entries[35].y=-0.5936;
M->NT.Entries[36].y=-0.8639;
M->NT.Entries[37].y=-0.9103;
M->NT.Entries[38].y=-0.9103;
M->NT.Entries[39].y=-0.9762;
M->NT.Entries[40].y=-0.7508;
M->NT.Entries[41].y=-0.5942;
M->NT.Entries[42].y=-0.7688;
M->NT.Entries[43].y=-0.5936;
M->NT.Entries[44].y=-0.7508;
M->NT.Entries[45].y=-0.7688;
M->NT.Entries[46].y=-0.5942;
M->NT.Entries[47].y=-0.5936;
M->NT.Entries[48].y=-0.8639;
M->NT.Entries[49].y=-0.9103;
M->NT.Entries[50].y=-0.9103;
M->NT.Entries[51].y=-0.9762;
M->NT.Entries[52].y=-0.7508;
M->NT.Entries[53].y=-0.5942;
M->NT.Entries[54].y=-0.7688;
M->NT.Entries[55].y=-0.5936;
M->NT.Entries[56].y=-0.7508;
M->NT.Entries[57].y=-0.7688;
M->NT.Entries[58].y=-0.5942;
M->NT.Entries[59].y=-0.5936;
M->NT.Entries[60].y=-0.2472;
M->NT.Entries[61].y=-0.4435;
M->NT.Entries[62].y=-0.1611;
M->NT.Entries[63].y=-0.3765;
M->NT.Entries[64].y=-0.2472;
M->NT.Entries[65].y=-0.1611;
M->NT.Entries[66].y=-0.4435;
M->NT.Entries[67].y=-0.3765;
M->NT.Entries[68].y=-0.0641;
M->NT.Entries[69].y=0.0679;
M->NT.Entries[70].y=0.0679;
M->NT.Entries[71].y=0.2424;
M->NT.Entries[72].y=-0.2472;
M->NT.Entries[73].y=-0.4435;
M->NT.Entries[74].y=-0.1611;
M->NT.Entries[75].y=-0.3765;
M->NT.Entries[76].y=-0.2472;
M->NT.Entries[77].y=-0.1611;
M->NT.Entries[78].y=-0.4435;
M->NT.Entries[79].y=-0.3765;
M->NT.Entries[80].y=-0.0641;
M->NT.Entries[81].y=0.0679;
M->NT.Entries[82].y=0.0679;
M->NT.Entries[83].y=0.2424;
M->NT.Entries[84].y=-0.2472;
M->NT.Entries[85].y=-0.4435;
M->NT.Entries[86].y=-0.1611;
M->NT.Entries[87].y=-0.3765;
M->NT.Entries[88].y=-0.2472;
M->NT.Entries[89].y=-0.1611;
M->NT.Entries[90].y=-0.4435;
M->NT.Entries[91].y=-0.3765;
M->NT.Entries[92].y=-0.0641;
M->NT.Entries[93].y=0.0679;
M->NT.Entries[94].y=0.0679;
M->NT.Entries[95].y=0.2424;
M->NT.Entries[96].y=-0.2472;
M->NT.Entries[97].y=-0.4435;
M->NT.Entries[98].y=-0.1611;
M->NT.Entries[99].y=-0.3765;
M->NT.Entries[100].y=-0.2472;
M->NT.Entries[101].y=-0.1611;
M->NT.Entries[102].y=-0.4435;
M->NT.Entries[103].y=-0.3765;
M->NT.Entries[104].y=-0.0641;
M->NT.Entries[105].y=0.0679;
M->NT.Entries[106].y=0.0679;
M->NT.Entries[107].y=0.2424;
M->NT.Entries[108].y=-0.2472;
M->NT.Entries[109].y=-0.4435;
M->NT.Entries[110].y=-0.1611;
M->NT.Entries[111].y=-0.3765;
M->NT.Entries[112].y=-0.2472;
M->NT.Entries[113].y=-0.1611;
M->NT.Entries[114].y=-0.4435;
M->NT.Entries[115].y=-0.3765;
M->NT.Entries[116].y=-0.0641;
M->NT.Entries[117].y=0.0679;
M->NT.Entries[118].y=0.0679;
M->NT.Entries[119].y=0.2424;
M->NT.Entries[120].y=0.0641;
M->NT.Entries[121].y=-0.0679;
M->NT.Entries[122].y=-0.0679;
M->NT.Entries[123].y=-0.2424;
M->NT.Entries[124].y=0.2472;
M->NT.Entries[125].y=0.4435;
M->NT.Entries[126].y=0.1611;
M->NT.Entries[127].y=0.3765;
M->NT.Entries[128].y=0.2472;
M->NT.Entries[129].y=0.1611;
M->NT.Entries[130].y=0.4435;
M->NT.Entries[131].y=0.3765;
M->NT.Entries[132].y=0.0641;
M->NT.Entries[133].y=-0.0679;
M->NT.Entries[134].y=-0.0679;
M->NT.Entries[135].y=-0.2424;
M->NT.Entries[136].y=0.2472;
M->NT.Entries[137].y=0.4435;
M->NT.Entries[138].y=0.1611;
M->NT.Entries[139].y=0.3765;
M->NT.Entries[140].y=0.2472;
M->NT.Entries[141].y=0.1611;
M->NT.Entries[142].y=0.4435;
M->NT.Entries[143].y=0.3765;
M->NT.Entries[144].y=0.0641;
M->NT.Entries[145].y=-0.0679;
M->NT.Entries[146].y=-0.0679;
M->NT.Entries[147].y=-0.2424;
M->NT.Entries[148].y=0.2472;
M->NT.Entries[149].y=0.4435;
M->NT.Entries[150].y=0.1611;
M->NT.Entries[151].y=0.3765;
M->NT.Entries[152].y=0.2472;
M->NT.Entries[153].y=0.1611;
M->NT.Entries[154].y=0.4435;
M->NT.Entries[155].y=0.3765;
M->NT.Entries[156].y=0.0641;
M->NT.Entries[157].y=-0.0679;
M->NT.Entries[158].y=-0.0679;
M->NT.Entries[159].y=-0.2424;
M->NT.Entries[160].y=0.2472;
M->NT.Entries[161].y=0.4435;
M->NT.Entries[162].y=0.1611;
M->NT.Entries[163].y=0.3765;
M->NT.Entries[164].y=0.2472;
M->NT.Entries[165].y=0.1611;
M->NT.Entries[166].y=0.4435;
M->NT.Entries[167].y=0.3765;
M->NT.Entries[168].y=0.0641;
M->NT.Entries[169].y=-0.0679;
M->NT.Entries[170].y=-0.0679;
M->NT.Entries[171].y=-0.2424;
M->NT.Entries[172].y=0.2472;
M->NT.Entries[173].y=0.4435;
M->NT.Entries[174].y=0.1611;
M->NT.Entries[175].y=0.3765;
M->NT.Entries[176].y=0.2472;
M->NT.Entries[177].y=0.1611;
M->NT.Entries[178].y=0.4435;
M->NT.Entries[179].y=0.3765;
M->NT.Entries[180].y=0.7508;
M->NT.Entries[181].y=0.5942;
M->NT.Entries[182].y=0.7688;
M->NT.Entries[183].y=0.5936;
M->NT.Entries[184].y=0.7508;
M->NT.Entries[185].y=0.7688;
M->NT.Entries[186].y=0.5942;
M->NT.Entries[187].y=0.5936;
M->NT.Entries[188].y=0.8639;
M->NT.Entries[189].y=0.9103;
M->NT.Entries[190].y=0.9103;
M->NT.Entries[191].y=0.9762;
M->NT.Entries[192].y=0.7508;
M->NT.Entries[193].y=0.5942;
M->NT.Entries[194].y=0.7688;
M->NT.Entries[195].y=0.5936;
M->NT.Entries[196].y=0.7508;
M->NT.Entries[197].y=0.7688;
M->NT.Entries[198].y=0.5942;
M->NT.Entries[199].y=0.5936;
M->NT.Entries[200].y=0.8639;
M->NT.Entries[201].y=0.9103;
M->NT.Entries[202].y=0.9103;
M->NT.Entries[203].y=0.9762;
M->NT.Entries[204].y=0.7508;
M->NT.Entries[205].y=0.5942;
M->NT.Entries[206].y=0.7688;
M->NT.Entries[207].y=0.5936;
M->NT.Entries[208].y=0.7508;
M->NT.Entries[209].y=0.7688;
M->NT.Entries[210].y=0.5942;
M->NT.Entries[211].y=0.5936;
M->NT.Entries[212].y=0.8639;
M->NT.Entries[213].y=0.9103;
M->NT.Entries[214].y=0.9103;
M->NT.Entries[215].y=0.9762;
M->NT.Entries[216].y=0.7508;
M->NT.Entries[217].y=0.5942;
M->NT.Entries[218].y=0.7688;
M->NT.Entries[219].y=0.5936;
M->NT.Entries[220].y=0.7508;
M->NT.Entries[221].y=0.7688;
M->NT.Entries[222].y=0.5942;
M->NT.Entries[223].y=0.5936;
M->NT.Entries[224].y=0.8639;
M->NT.Entries[225].y=0.9103;
M->NT.Entries[226].y=0.9103;
M->NT.Entries[227].y=0.9762;
M->NT.Entries[228].y=0.7508;
M->NT.Entries[229].y=0.5942;
M->NT.Entries[230].y=0.7688;
M->NT.Entries[231].y=0.5936;
M->NT.Entries[232].y=0.7508;
M->NT.Entries[233].y=0.7688;
M->NT.Entries[234].y=0.5942;
M->NT.Entries[235].y=0.5936;
M->NT.Entries[236].y=0.8639;
M->NT.Entries[237].y=0.9103;
M->NT.Entries[238].y=0.9103;
M->NT.Entries[239].y=0.9762;

M->NT.Entries[0].z=0.479;
M->NT.Entries[1].z=0.3094;
M->NT.Entries[2].z=0.412;
M->NT.Entries[3].z=0.2065;
M->NT.Entries[4].z=0.5866;
M->NT.Entries[5].z=0.7125;
M->NT.Entries[6].z=0.444;
M->NT.Entries[7].z=0.5703;
M->NT.Entries[8].z=0.6531;
M->NT.Entries[9].z=0.6297;
M->NT.Entries[10].z=0.7957;
M->NT.Entries[11].z=0.7951;
M->NT.Entries[12].z=0.1076;
M->NT.Entries[13].z=0.3006;
M->NT.Entries[14].z=0.1346;
M->NT.Entries[15].z=0.3638;
M->NT.Entries[16].z=0;
M->NT.Entries[17].z=-0.166;
M->NT.Entries[18].z=0.166;
M->NT.Entries[19].z=0;
M->NT.Entries[20].z=-0.1076;
M->NT.Entries[21].z=-0.1346;
M->NT.Entries[22].z=-0.3006;
M->NT.Entries[23].z=-0.3638;
M->NT.Entries[24].z=0.296;
M->NT.Entries[25].z=0.3572;
M->NT.Entries[26].z=0.0886;
M->NT.Entries[27].z=0.1276;
M->NT.Entries[28].z=0.4702;
M->NT.Entries[29].z=0.575;
M->NT.Entries[30].z=0.575;
M->NT.Entries[31].z=0.7162;
M->NT.Entries[32].z=0.296;
M->NT.Entries[33].z=0.0886;
M->NT.Entries[34].z=0.3572;
M->NT.Entries[35].z=0.1276;
M->NT.Entries[36].z=-0.296;
M->NT.Entries[37].z=-0.0886;
M->NT.Entries[38].z=-0.3572;
M->NT.Entries[39].z=-0.1276;
M->NT.Entries[40].z=-0.296;
M->NT.Entries[41].z=-0.3572;
M->NT.Entries[42].z=-0.0886;
M->NT.Entries[43].z=-0.1276;
M->NT.Entries[44].z=-0.4702;
M->NT.Entries[45].z=-0.575;
M->NT.Entries[46].z=-0.575;
M->NT.Entries[47].z=-0.7162;
M->NT.Entries[48].z=-0.479;
M->NT.Entries[49].z=-0.412;
M->NT.Entries[50].z=-0.3094;
M->NT.Entries[51].z=-0.2065;
M->NT.Entries[52].z=-0.6531;
M->NT.Entries[53].z=-0.7957;
M->NT.Entries[54].z=-0.6297;
M->NT.Entries[55].z=-0.7951;
M->NT.Entries[56].z=-0.5866;
M->NT.Entries[57].z=-0.444;
M->NT.Entries[58].z=-0.7125;
M->NT.Entries[59].z=-0.5703;
M->NT.Entries[60].z=0.1076;
M->NT.Entries[61].z=0.1346;
M->NT.Entries[62].z=0.3006;
M->NT.Entries[63].z=0.3638;
M->NT.Entries[64].z=-0.1076;
M->NT.Entries[65].z=-0.3006;
M->NT.Entries[66].z=-0.1346;
M->NT.Entries[67].z=-0.3638;
M->NT.Entries[68].z=0;
M->NT.Entries[69].z=0.166;
M->NT.Entries[70].z=-0.166;
M->NT.Entries[71].z=0;
M->NT.Entries[72].z=0.9491;
M->NT.Entries[73].z=0.8843;
M->NT.Entries[74].z=0.9869;
M->NT.Entries[75].z=0.9227;
M->NT.Entries[76].z=0.8826;
M->NT.Entries[77].z=0.8012;
M->NT.Entries[78].z=0.8012;
M->NT.Entries[79].z=0.6979;
M->NT.Entries[80].z=0.9491;
M->NT.Entries[81].z=0.9869;
M->NT.Entries[82].z=0.8843;
M->NT.Entries[83].z=0.9227;
M->NT.Entries[84].z=0.479;
M->NT.Entries[85].z=0.412;
M->NT.Entries[86].z=0.3094;
M->NT.Entries[87].z=0.2065;
M->NT.Entries[88].z=0.6531;
M->NT.Entries[89].z=0.7957;
M->NT.Entries[90].z=0.6297;
M->NT.Entries[91].z=0.7951;
M->NT.Entries[92].z=0.5866;
M->NT.Entries[93].z=0.444;
M->NT.Entries[94].z=0.7125;
M->NT.Entries[95].z=0.5703;
M->NT.Entries[96].z=-0.6531;
M->NT.Entries[97].z=-0.6297;
M->NT.Entries[98].z=-0.7957;
M->NT.Entries[99].z=-0.7951;
M->NT.Entries[100].z=-0.479;
M->NT.Entries[101].z=-0.3094;
M->NT.Entries[102].z=-0.412;
M->NT.Entries[103].z=-0.2065;
M->NT.Entries[104].z=-0.5866;
M->NT.Entries[105].z=-0.7125;
M->NT.Entries[106].z=-0.444;
M->NT.Entries[107].z=-0.5703;
M->NT.Entries[108].z=-0.8826;
M->NT.Entries[109].z=-0.8012;
M->NT.Entries[110].z=-0.8012;
M->NT.Entries[111].z=-0.6979;
M->NT.Entries[112].z=-0.9491;
M->NT.Entries[113].z=-0.9869;
M->NT.Entries[114].z=-0.8843;
M->NT.Entries[115].z=-0.9227;
M->NT.Entries[116].z=-0.9491;
M->NT.Entries[117].z=-0.8843;
M->NT.Entries[118].z=-0.9869;
M->NT.Entries[119].z=-0.9227;
M->NT.Entries[120].z=0.5866;
M->NT.Entries[121].z=0.444;
M->NT.Entries[122].z=0.7125;
M->NT.Entries[123].z=0.5703;
M->NT.Entries[124].z=0.479;
M->NT.Entries[125].z=0.412;
M->NT.Entries[126].z=0.3094;
M->NT.Entries[127].z=0.2065;
M->NT.Entries[128].z=0.6531;
M->NT.Entries[129].z=0.7957;
M->NT.Entries[130].z=0.6297;
M->NT.Entries[131].z=0.7951;
M->NT.Entries[132].z=0.9491;
M->NT.Entries[133].z=0.9869;
M->NT.Entries[134].z=0.8843;
M->NT.Entries[135].z=0.9227;
M->NT.Entries[136].z=0.9491;
M->NT.Entries[137].z=0.8843;
M->NT.Entries[138].z=0.9869;
M->NT.Entries[139].z=0.9227;
M->NT.Entries[140].z=0.8826;
M->NT.Entries[141].z=0.8012;
M->NT.Entries[142].z=0.8012;
M->NT.Entries[143].z=0.6979;
M->NT.Entries[144].z=0;
M->NT.Entries[145].z=0.166;
M->NT.Entries[146].z=-0.166;
M->NT.Entries[147].z=0;
M->NT.Entries[148].z=0.1076;
M->NT.Entries[149].z=0.1346;
M->NT.Entries[150].z=0.3006;
M->NT.Entries[151].z=0.3638;
M->NT.Entries[152].z=-0.1076;
M->NT.Entries[153].z=-0.3006;
M->NT.Entries[154].z=-0.1346;
M->NT.Entries[155].z=-0.3638;
M->NT.Entries[156].z=-0.9491;
M->NT.Entries[157].z=-0.8843;
M->NT.Entries[158].z=-0.9869;
M->NT.Entries[159].z=-0.9227;
M->NT.Entries[160].z=-0.8826;
M->NT.Entries[161].z=-0.8012;
M->NT.Entries[162].z=-0.8012;
M->NT.Entries[163].z=-0.6979;
M->NT.Entries[164].z=-0.9491;
M->NT.Entries[165].z=-0.9869;
M->NT.Entries[166].z=-0.8843;
M->NT.Entries[167].z=-0.9227;
M->NT.Entries[168].z=-0.5866;
M->NT.Entries[169].z=-0.7125;
M->NT.Entries[170].z=-0.444;
M->NT.Entries[171].z=-0.5703;
M->NT.Entries[172].z=-0.6531;
M->NT.Entries[173].z=-0.6297;
M->NT.Entries[174].z=-0.7957;
M->NT.Entries[175].z=-0.7951;
M->NT.Entries[176].z=-0.479;
M->NT.Entries[177].z=-0.3094;
M->NT.Entries[178].z=-0.412;
M->NT.Entries[179].z=-0.2065;
M->NT.Entries[180].z=0.4702;
M->NT.Entries[181].z=0.575;
M->NT.Entries[182].z=0.575;
M->NT.Entries[183].z=0.7162;
M->NT.Entries[184].z=0.296;
M->NT.Entries[185].z=0.0886;
M->NT.Entries[186].z=0.3572;
M->NT.Entries[187].z=0.1276;
M->NT.Entries[188].z=0.296;
M->NT.Entries[189].z=0.3572;
M->NT.Entries[190].z=0.0886;
M->NT.Entries[191].z=0.1276;
M->NT.Entries[192].z=0.5866;
M->NT.Entries[193].z=0.7125;
M->NT.Entries[194].z=0.444;
M->NT.Entries[195].z=0.5703;
M->NT.Entries[196].z=0.6531;
M->NT.Entries[197].z=0.6297;
M->NT.Entries[198].z=0.7957;
M->NT.Entries[199].z=0.7951;
M->NT.Entries[200].z=0.479;
M->NT.Entries[201].z=0.3094;
M->NT.Entries[202].z=0.412;
M->NT.Entries[203].z=0.2065;
M->NT.Entries[204].z=-0.1076;
M->NT.Entries[205].z=-0.1346;
M->NT.Entries[206].z=-0.3006;
M->NT.Entries[207].z=-0.3638;
M->NT.Entries[208].z=0.1076;
M->NT.Entries[209].z=0.3006;
M->NT.Entries[210].z=0.1346;
M->NT.Entries[211].z=0.3638;
M->NT.Entries[212].z=0;
M->NT.Entries[213].z=-0.166;
M->NT.Entries[214].z=0.166;
M->NT.Entries[215].z=0;
M->NT.Entries[216].z=-0.6531;
M->NT.Entries[217].z=-0.7957;
M->NT.Entries[218].z=-0.6297;
M->NT.Entries[219].z=-0.7951;
M->NT.Entries[220].z=-0.5866;
M->NT.Entries[221].z=-0.444;
M->NT.Entries[222].z=-0.7125;
M->NT.Entries[223].z=-0.5703;
M->NT.Entries[224].z=-0.479;
M->NT.Entries[225].z=-0.412;
M->NT.Entries[226].z=-0.3094;
M->NT.Entries[227].z=-0.2065;
M->NT.Entries[228].z=-0.296;
M->NT.Entries[229].z=-0.3572;
M->NT.Entries[230].z=-0.0886;
M->NT.Entries[231].z=-0.1276;
M->NT.Entries[232].z=-0.4702;
M->NT.Entries[233].z=-0.575;
M->NT.Entries[234].z=-0.575;
M->NT.Entries[235].z=-0.7162;
M->NT.Entries[236].z=-0.296;
M->NT.Entries[237].z=-0.0886;
M->NT.Entries[238].z=-0.3572;
M->NT.Entries[239].z=-0.1276;


    return M;
}

void Load_SE3_Model(struct _SE3_Model* Model, const char* Filename)
{
    FILE * file;
    file = fopen(Filename,"rb");
    if(!file)
    {
        file = fopen("resources/artwork/placeholders/no_model.se3", "rb");
        printf("ERROR: COULD NOT LOAD FILE: ");
        printf(Filename);
        printf("\n");
    }

    unsigned char header[68];

    fread(header, 1, 68, file);

    Model->Width = *(int*)&(header[8]);
    Model->Height =*(int*)&(header[12]);
    Model->Frame_Size =*(int*)&(header[16]);
    Model->Number_Of_Textures =*(int*)&(header[20]);
    Model->Number_Of_Vertices =*(int*)&(header[24]);
    Model->Number_Of_Texture_Vertices =*(int*)&(header[28]);
    Model->Number_Of_Triangles =*(int*)&(header[32]);
    Model->Number_Of_Commands =*(int*)&(header[36]);
    Model->Number_Of_Frames =*(int*)&(header[40]);

    int Texture_Name_Offset=*(int*)&(header[44]);
    int Texture_Vertex_Offset=*(int*)&(header[48]);
    int Triangle_Offset=*(int*)&(header[52]);
    int Frame_Data_Offset=*(int*)&(header[56]);
    int Command_Offset=*(int*)&(header[60]);
    int EndOF = *(int*)&(header[64]);

    Model->Texture_Names = (struct Texture_Name *) calloc (Model->Number_Of_Textures,sizeof (struct Texture_Name));
    Model->Texture_Coordinates = (struct Texture_Coordinate *) calloc (Model->Number_Of_Texture_Vertices,sizeof (struct Texture_Coordinate));
    Model->Triangle_Indices = (struct Triangle *) calloc (Model->Number_Of_Triangles,sizeof (struct Triangle));
    Model->Frame_Data = (struct Frame *) calloc (Model->Number_Of_Frames,sizeof (struct Frame));
    Model->Command_Array = (int *)calloc (Model->Number_Of_Commands,sizeof (int));



  /* Read model data */
  fseek (file, Texture_Name_Offset, SEEK_SET);
  fread (Model->Texture_Names, sizeof (struct Texture_Name),
	 Model->Number_Of_Textures, file);

  fseek (file, Texture_Vertex_Offset, SEEK_SET);
  fread (Model->Texture_Coordinates, sizeof (struct Texture_Coordinate),
	 Model->Number_Of_Texture_Vertices, file);

  fseek (file, Triangle_Offset, SEEK_SET);
  fread (Model->Triangle_Indices, sizeof (struct Triangle),
	 Model->Number_Of_Triangles, file);

  fseek (file, Command_Offset, SEEK_SET);
  fread (Model->Command_Array, sizeof (int), Model->Number_Of_Commands, file);



  /* Read frames */
  fseek (file, Frame_Data_Offset, SEEK_SET);
  for (int j = 0; j < Model->Number_Of_Frames; j++)
    {
      /* Memory allocation for vertices of this frame */
      Model->Frame_Data[j].First_Vertex = (struct Vertex *)
	calloc (Model->Number_Of_Vertices,sizeof (struct Vertex));

      /* Read frame data */
      fread (Model->Frame_Data[j].Frame_Scale, sizeof (Vector), 1, file);
      fread (Model->Frame_Data[j].Frame_Translation_Vector, sizeof (Vector), 1, file);
      fread (Model->Frame_Data[j].Frame_Name, sizeof (char), 16, file);
      fread (Model->Frame_Data[j].First_Vertex, sizeof (struct Vertex),
	     Model->Number_Of_Vertices, file);
    }

  fclose (file);

    int Number_of_Vertices = Model->Number_Of_Triangles*3;

    //Model->glDraw_Vertices = (gl_vertex_t*)calloc(Number_of_Vertices, sizeof(gl_vertex_t));
    Model->Draw_Arrays = (struct _Draw_Array*)calloc(Model->Number_Of_Frames, sizeof(struct _Draw_Array));

    for(int n = 0; n < Model->Number_Of_Frames; n++) //For each sub model
    {
        Model->Draw_Arrays[n].Triangle_Coordinates = (float*)calloc(9*Model->Number_Of_Triangles,sizeof(float)); //Each vertex has 3 Coords!
        Model->Draw_Arrays[n].Texture_Triangle_Coordinates = (float*)calloc(6*Model->Number_Of_Triangles,sizeof(float));
        Model->Draw_Arrays[n].Triangle_Normal_Coordinates = (float*)calloc(3*Model->Number_Of_Triangles,sizeof(float));
    }
    Extract_All_Frames_To_Arrays(Model);
};

float Convert_Unsigned_Byte_To_Float(unsigned char Byte, float Scale, float Translate)
{

}

void Load_SE3_Texture(struct _SE3_Model* Image, const char* Filename)
{
    FILE * file;
    file = fopen(Filename,"rb");
    if(!file)
    {
        file = fopen("resources/artwork/placeholders/no_texture.bmp", "rb");
    }

    unsigned char header[54];
    fread(header, 1, 54, file);
    int RGB_Image_Size = ((256) * (256) * 3);
    int RGBA_Image_Size = ((256) * (256) * 4);
    fseek(file, 54, SEEK_SET);
    Image->RGB_Canvas = (unsigned char*) calloc (RGB_Image_Size , sizeof(unsigned char));
    Image->RGBA_Canvas = (unsigned char*) calloc (RGBA_Image_Size , sizeof(unsigned char));
    memset(Image->RGBA_Canvas, 0, RGBA_Image_Size);
    fread(Image->RGB_Canvas,1,RGB_Image_Size,file);
    fclose(file);
    int in = 0;
    int r = 0;
    for(int j = 0; j < (RGBA_Image_Size/4); j++)
    {
        Image->RGBA_Canvas[r] = Image->RGB_Canvas[in+2];
        Image->RGBA_Canvas[r + 1] = Image->RGB_Canvas[in + 1];
        Image->RGBA_Canvas[r + 2] = Image->RGB_Canvas[in];
        Image->RGBA_Canvas[r + 3] = 255;
        if(Image->RGBA_Canvas[r] == 200)
        {
            if(Image->RGBA_Canvas[r+1] == 200)
            {
                if(Image->RGBA_Canvas[r + 2] == 200)
                {
                    Image->RGBA_Canvas[r + 3] = 0;
                }
            }
        }
        in += 3;
        r += 4;
    }

    glGenTextures(1, &Image->Texture_ID);
    glBindTexture(GL_TEXTURE_2D, Image->Texture_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Clamp to edge to avoid issues with non-power-of-2 textures
 // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
 //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256,256, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image->RGBA_Canvas);
    glEnable(GL_TEXTURE_2D);
}

void Load_Transparent_SE3_Texture(struct _SE3_Model* Image, const char* Filename, const char* Transparency_Map_Filename)
{
    FILE * file;
    file = fopen(Filename,"rb");
    if(!file)
    {
        file = fopen("resources/artwork/placeholders/no_texture.bmp", "rb");
    }

    unsigned char header[54];
    fread(header, 1, 54, file);
    int RGB_Image_Size = ((256) * (256) * 3);
    int RGBA_Image_Size = ((256) * (256) * 4);
    fseek(file, 54, SEEK_SET);
    Image->RGB_Canvas = (unsigned char*) calloc (RGB_Image_Size , sizeof(unsigned char));
    Image->RGBA_Canvas = (unsigned char*) calloc (RGBA_Image_Size , sizeof(unsigned char));
    memset(Image->RGBA_Canvas, 0, RGBA_Image_Size);
    fread(Image->RGB_Canvas,1,RGB_Image_Size,file);
    fclose(file);







    FILE * filetwo;
    filetwo = fopen(Transparency_Map_Filename,"rb");
    if(!filetwo)
    {
        filetwo = fopen("resources/artwork/placeholders/no_texture.bmp", "rb");
    }

    unsigned char headertwo[54];
    fread(headertwo, 1, 54, filetwo);
    fseek(filetwo, 54, SEEK_SET);
    unsigned char* Transparency_Buffer = (unsigned char*) calloc (RGB_Image_Size , sizeof(unsigned char));
    fread(Transparency_Buffer,1,RGB_Image_Size,filetwo);
    fclose(filetwo);

    int in = 0;
    int r = 0;
    for(int j = 0; j < (RGBA_Image_Size/4); j++)
    {
        Image->RGBA_Canvas[r] = Image->RGB_Canvas[in+2];
        Image->RGBA_Canvas[r + 1] = Image->RGB_Canvas[in + 1];
        Image->RGBA_Canvas[r + 2] = Image->RGB_Canvas[in];
        Image->RGBA_Canvas[r + 3] = Transparency_Buffer[in];
        in += 3;
        r += 4;
    }

    glGenTextures(1, &Image->Texture_ID);
    glBindTexture(GL_TEXTURE_2D, Image->Texture_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Clamp to edge to avoid issues with non-power-of-2 textures
 // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
 //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256,256, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image->RGBA_Canvas);
    glEnable(GL_TEXTURE_2D);
}


void Interpolate_SE3_Model(struct _SE3_Model* Model, Vector *vertlist )
{
    Vector  *curr_v;    // pointeur to current frame vertices
    Vector  *next_v;    // pointeur to next frame vertices

    // create current frame and next frame's vertex list
    // from the whole vertex list
    curr_v = &Model->Vertex_Array[ Model->Number_Of_Vertices * Model->Animation_State.Current_Frame_ID];
    next_v = &Model->Vertex_Array[ Model->Number_Of_Vertices * Model->Animation_State.Next_Frame_ID ];

    // interpolate and scale vertices to avoid ugly animation
    for( int i = 0; i < Model->Number_Of_Vertices ; i++ )
    {
        vertlist[i][0] = (curr_v[i][0] + Model->Animation_State.Interpolation_Percentage * (next_v[i][0] - curr_v[i][0])) * Model->Model_Scale;
        vertlist[i][1] = (curr_v[i][1] + Model->Animation_State.Interpolation_Percentage * (next_v[i][1] - curr_v[i][1])) * Model->Model_Scale;
        vertlist[i][2] = (curr_v[i][2] + Model->Animation_State.Interpolation_Percentage * (next_v[i][2] - curr_v[i][2])) * Model->Model_Scale;


    }
}

void Render_SE3_Model (struct _SE3_Model* Model, int n, float x, float y, float z, float x_Rotation, float y_Rotation, float z_Rotation, bool Vertex_Coloring, float R, float G, float B)
{
    float Color[3];
    Color[0] = R;
    Color[1] = G;
    Color[2] = B;

    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    int i, j;
    GLfloat s, t;
    Vector v;
    struct Frame *pframe;
    struct Vertex *pvert;
  /* Check if n is in a valid range */
    if ((n < 0) || (n > Model->Number_Of_Frames - 1))
    return;

  /* Enable model's texture */
    glBindTexture (GL_TEXTURE_2D, Model->Texture_ID);

  /* Draw the model */
    glBegin (GL_TRIANGLES);
    /* Draw each triangle */
    for (int m = 0; m < Model->Number_Of_Triangles; m++)
    {
	/* Draw each vertex */
	for (int j = 0; j < 3; j++)
    {
        pframe = &Model->Frame_Data[n];
        pvert = &pframe->First_Vertex[Model->Triangle_Indices[m].Triangle_Vertex_IDs[j]];
	    /* Compute texture coordinates */
	    t = (float)Model->Texture_Coordinates[Model->Triangle_Indices[m].Triangle_Texture_Vertex_IDs[j]].x / 256;
	    s = (float)Model->Texture_Coordinates[Model->Triangle_Indices[m].Triangle_Texture_Vertex_IDs[j]].y / 256;
	    /* Pass texture coordinates to OpenGL */
	    glTexCoord2f (s, t);
	    /* Nrmal vector */
	    Vector Normal_ID;
	    Normal_ID[0] = Model->NT.Entries[pvert->Normal_Index].x;
	    Normal_ID[1] = Model->NT.Entries[pvert->Normal_Index].y;
	    Normal_ID[2] = Model->NT.Entries[pvert->Normal_Index].z;

	    glNormal3fv(Normal_ID);

	   // for(int j = 0; j < 240; j++)
	   // {
        //    printf("%f", Model->NT.Entries[pvert->Normal_Index].x);
        //    printf("\n");
	   // }
	    /* Calculate vertex real position */
	    v[0] = (pframe->Frame_Scale[0] * pvert->Coordinates[0]) + pframe->Frame_Translation_Vector[0];
	    v[1] = (pframe->Frame_Scale[1] * pvert->Coordinates[1]) + pframe->Frame_Translation_Vector[1];
	    v[2] = (pframe->Frame_Scale[2] * pvert->Coordinates[2]) + pframe->Frame_Translation_Vector[2];
	    if(Vertex_Coloring)
	    {
           // glColor3f(Color[0],Color[1],Color[2]);
	    }
	    glVertex3fv (v);
	  }
	  //drawBillboard();
      }
  glEnd();
 // glPopMatrix();
}


void Render_SE3_Model_Dynamic_Texture(struct _SE3_Model* Model, int n, float x, float y, float z, float x_Rotation, float y_Rotation, float z_Rotation, bool Vertex_Coloring, float R, float G, float B, int Texture_ID)
{
    float Color[3];
    Color[0] = R;
    Color[1] = G;
    Color[2] = B;

    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    int i, j;
    GLfloat s, t;
    Vector v;
    struct Frame *pframe;
    struct Vertex *pvert;
  /* Check if n is in a valid range */
    if ((n < 0) || (n > Model->Number_Of_Frames - 1))
    return;

  /* Enable model's texture */
    glBindTexture (GL_TEXTURE_2D, Texture_ID);

  /* Draw the model */
    glBegin (GL_TRIANGLES);
    /* Draw each triangle */
    for (int m = 0; m < Model->Number_Of_Triangles; m++)
    {
	/* Draw each vertex */
	for (int j = 0; j < 3; j++)
    {
        pframe = &Model->Frame_Data[n];
        pvert = &pframe->First_Vertex[Model->Triangle_Indices[m].Triangle_Vertex_IDs[j]];
	    /* Compute texture coordinates */
	    t = (float)Model->Texture_Coordinates[Model->Triangle_Indices[m].Triangle_Texture_Vertex_IDs[j]].x / 256;
	    s = (float)Model->Texture_Coordinates[Model->Triangle_Indices[m].Triangle_Texture_Vertex_IDs[j]].y / 256;
	    /* Pass texture coordinates to OpenGL */
	    glTexCoord2f (s, t);
	    /* Nrmal vector */
	    glNormal3fv (Model->anorms_table[pvert->Normal_Index]);
	    /* Calculate vertex real position */
	    v[0] = (pframe->Frame_Scale[0] * pvert->Coordinates[0]) + pframe->Frame_Translation_Vector[0];
	    v[1] = (pframe->Frame_Scale[1] * pvert->Coordinates[1]) + pframe->Frame_Translation_Vector[1];
	    v[2] = (pframe->Frame_Scale[2] * pvert->Coordinates[2]) + pframe->Frame_Translation_Vector[2];
	    if(Vertex_Coloring)
	    {
           // glColor3f(Color[0],Color[1],Color[2]);
	    }
	    glVertex3fv (v);
	  }
	  //drawBillboard();
      }
  glEnd();
 // glPopMatrix();
}


void Add_Animation(struct _Animation_File* A, unsigned char Index, char Name[32], unsigned char Keyframe_One, unsigned char Keyframe_Two, float Speed, bool Loop)
{
for(int j = 0; j <32; j++)
{
    A->Animations[Index].Name[j] = Name[j];
    }
     A->Animations[Index].Name[31] = '\0';

    A->Animations[Index].Keyframes[0] = Keyframe_One;
    A->Animations[Index].Keyframes[1] = Keyframe_Two;



    A->Animations[Index].Speed = Speed;
    A->Animations[Index].Loop = Loop;

}
void Create_Animation_File(const char* Filename)
{
    struct _Animation_File A;
    char Name[32];
    sprintf(Name, "BLUESPIKE");

    Add_Animation(&A, 0, Name, 0, 1, 1, true);

    FILE* f = fopen(Filename, "wb");
    fwrite(&A, 1, sizeof(struct _Animation_File), f);
    fclose(f);
}

void Load_SE3_Animation_Array(struct _SE3_Model* Model, const char* Filename)
{
    FILE* f = fopen(Filename, "rb");
    fread(Model->Animation_File, 1, sizeof(struct _Animation_File), f);
    fclose(f);
}


void Render_Wrapper(struct _SE3_Model* Model, unsigned char Index, unsigned char Keyframe_One, unsigned char Keyframe_Two, float Speed, bool Loop, int n, float x, float y, float z, float x_Rotation, float y_Rotation, float z_Rotation) //for animations
{
    //Model->Current_Keyframe = Keyframe_One;


    //Render_SE3_Model_GLDraw(Model, Model->Current_Keyframe, x, y, z, x_Rotation, y_Rotation, z_Rotation,true,1,1,1);
    Model->Current_Keyframe += 1;
    if(Model->Current_Keyframe >= Model->Current_End_Keyframe)
    {
        Model->Current_Keyframe = Model->Current_Start_Keyframe;
    }
}


void Set_Model_Sequeance(struct _SE3_Model* Model, unsigned char Keyframe_One, unsigned char Keyframe_Two)
{
    Model->Current_Start_Keyframe = Keyframe_One;
    Model->Current_Keyframe = Keyframe_One;
    Model->Current_End_Keyframe = Keyframe_Two;
}


void Render_SE3_Model_GLDraw(struct _SE3_Model* Model, int n, float x, float y, float z, float x_Rotation, float y_Rotation, float z_Rotation, bool Vertex_Coloring, float R, float G, float B)
{
    glBindTexture (GL_TEXTURE_2D, Model->Texture_ID);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);

    glVertexPointer(3, GL_FLOAT, 0, Model->Draw_Arrays[n].Triangle_Coordinates);
        // Normal Pointer
    glNormalPointer(GL_FLOAT, 0, Model->Draw_Arrays[n].Triangle_Normal_Coordinates);
        // Texture Coordinate Pointer
    glTexCoordPointer(2, GL_FLOAT, 0, Model->Draw_Arrays[n].Texture_Triangle_Coordinates);
        // RENDER using glDrawArrays
    glDrawArrays(GL_TRIANGLES, 0, Model->Number_Of_Triangles);
        // Disable client-side arrays after drawing
    glDisableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
};

vec3_t calculate_frame_vertex_position(struct Frame *pframe, struct Vertex *pvert) {
    vec3_t pos;
    // Note: vertex->v[i] is now an unsigned short (2 bytes)
    pos.x = (pframe->Frame_Scale[0] * pvert->Coordinates[0]) + pframe->Frame_Translation_Vector[0];
    pos.y = (pframe->Frame_Scale[1] * pvert->Coordinates[1]) + pframe->Frame_Translation_Vector[1];
    pos.z = (pframe->Frame_Scale[2] * pvert->Coordinates[2]) + pframe->Frame_Translation_Vector[2];

   // printf("Frame Vertex Positions: %f %f %f\n",pos.x,pos.y,pos.z);

    return pos;
}

void update_gl_vertices_for_frame(struct _SE3_Model* Model, int frame_index) {
    struct Frame *pframe;
    struct Vertex *pvert;
    struct Vertex *pvert2;
    struct Vertex *pvert3;

    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    pframe = &Model->Frame_Data[frame_index];

    for (int i = 0; i < Model->Number_Of_Triangles; i++) {
        vec3_t p[3];

        pvert = &pframe->First_Vertex[Model->Triangle_Indices[i].Triangle_Vertex_IDs[0]];
        pvert2 = &pframe->First_Vertex[Model->Triangle_Indices[i].Triangle_Vertex_IDs[1]];
        pvert3 = &pframe->First_Vertex[Model->Triangle_Indices[i].Triangle_Vertex_IDs[2]];

        p[0] = calculate_frame_vertex_position(pframe, pvert);
        p[1] = calculate_frame_vertex_position(pframe, pvert2);
        p[2] = calculate_frame_vertex_position(pframe, pvert3);

        // 2. Calculate the face normal (N)
        vec3_t edge1 = vec3_t_sub(p[1], p[0]); // P2 - P1
        vec3_t edge2 = vec3_t_sub(p[2], p[0]); // P3 - P1
        vec3_t unnormalized_normal = vec3_t_cross(edge1, edge2);
        vec3_t normal = vec3_t_normalize(unnormalized_normal);

        // 3. Populate the gl_vertex_t array for all three vertices of the triangle
        for (int j = 0; j < 3; j++) {
            gl_vertex_t *v = &Model->glDraw_Vertices[i * 3 + j];

            // Position
            v->pos[0] = p[j].x;
            v->pos[1] = p[j].y;
            v->pos[2] = p[j].z;

            // Normal (using the single face normal for flat shading)
            v->normal[0] = normal.x;
            v->normal[1] = normal.y;
            v->normal[2] = normal.z;

            // UV Coordinates (normalized to 0.0-1.0 range)
            v->uv[0] = (float)Model->Texture_Coordinates[Model->Triangle_Indices[i].Triangle_Texture_Vertex_IDs[j]].x / 256;
            v->uv[1] = (float)Model->Texture_Coordinates[Model->Triangle_Indices[i].Triangle_Texture_Vertex_IDs[j]].y / 256;
        }
    }
}

void Extract_All_Frames_To_Arrays(struct _SE3_Model* Model)
{
    struct Frame *pframe;
    struct Vertex *pvert;
    for(int n = 0; n < Model->Number_Of_Frames; n++) //For each sub model
    {
        int Vertex_Counter = 0;
        int Normal_Counter = 0;
        int Texture_Vertex_Counter = 0;

        pframe = &Model->Frame_Data[n];
        for(int m = 0; m < Model->Number_Of_Triangles; m++)
        {
            vec3_t vOne;
            vec3_t vTwo;
            vec3_t vThree;

            GLfloat s, t;
            GLfloat sTwo, tTwo;
            GLfloat sThree, tThree;

            pvert = &pframe->First_Vertex[Model->Triangle_Indices[m].Triangle_Vertex_IDs[0]];
            t = (float)Model->Texture_Coordinates[Model->Triangle_Indices[m].Triangle_Texture_Vertex_IDs[0]].x / 256;
            s = (float)Model->Texture_Coordinates[Model->Triangle_Indices[m].Triangle_Texture_Vertex_IDs[0]].y / 256;
            vOne.x = (pframe->Frame_Scale[0] * pvert->Coordinates[0]) + pframe->Frame_Translation_Vector[0];
            vOne.y = (pframe->Frame_Scale[1] * pvert->Coordinates[1]) + pframe->Frame_Translation_Vector[1];
            vOne.z = (pframe->Frame_Scale[2] * pvert->Coordinates[2]) + pframe->Frame_Translation_Vector[2];

            pvert = &pframe->First_Vertex[Model->Triangle_Indices[m].Triangle_Vertex_IDs[1]];
            tTwo = (float)Model->Texture_Coordinates[Model->Triangle_Indices[m].Triangle_Texture_Vertex_IDs[1]].x / 256;
            sTwo = (float)Model->Texture_Coordinates[Model->Triangle_Indices[m].Triangle_Texture_Vertex_IDs[1]].y / 256;
            vTwo.x = (pframe->Frame_Scale[0] * pvert->Coordinates[0]) + pframe->Frame_Translation_Vector[0];
            vTwo.y = (pframe->Frame_Scale[1] * pvert->Coordinates[1]) + pframe->Frame_Translation_Vector[1];
            vTwo.z = (pframe->Frame_Scale[2] * pvert->Coordinates[2]) + pframe->Frame_Translation_Vector[2];

            pvert = &pframe->First_Vertex[Model->Triangle_Indices[m].Triangle_Vertex_IDs[2]];
            tThree = (float)Model->Texture_Coordinates[Model->Triangle_Indices[m].Triangle_Texture_Vertex_IDs[2]].x / 256;
            sThree = (float)Model->Texture_Coordinates[Model->Triangle_Indices[m].Triangle_Texture_Vertex_IDs[2]].y / 256;
            vThree.x = (pframe->Frame_Scale[0] * pvert->Coordinates[0]) + pframe->Frame_Translation_Vector[0];
            vThree.y = (pframe->Frame_Scale[1] * pvert->Coordinates[1]) + pframe->Frame_Translation_Vector[1];
            vThree.z = (pframe->Frame_Scale[2] * pvert->Coordinates[2]) + pframe->Frame_Translation_Vector[2];



            // 2. Calculate the face normal (N)
            vec3_t edge1 = vec3_t_sub(vTwo, vOne); // P2 - P1
            vec3_t edge2 = vec3_t_sub(vThree, vOne); // P3 - P1
            vec3_t unnormalized_normal = vec3_t_cross(edge1, edge2);
            vec3_t normal = vec3_t_normalize(unnormalized_normal);

            Model->Draw_Arrays[n].Triangle_Coordinates[Vertex_Counter]=vOne.x;
            Model->Draw_Arrays[n].Triangle_Coordinates[Vertex_Counter+1]=vOne.y;
            Model->Draw_Arrays[n].Triangle_Coordinates[Vertex_Counter+2]=vOne.z;

            Model->Draw_Arrays[n].Triangle_Coordinates[Vertex_Counter+3]=vTwo.x;
            Model->Draw_Arrays[n].Triangle_Coordinates[Vertex_Counter+4]=vTwo.y;
            Model->Draw_Arrays[n].Triangle_Coordinates[Vertex_Counter+5]=vTwo.z;

            Model->Draw_Arrays[n].Triangle_Coordinates[Vertex_Counter+6]=vThree.x;
            Model->Draw_Arrays[n].Triangle_Coordinates[Vertex_Counter+7]=vThree.y;
            Model->Draw_Arrays[n].Triangle_Coordinates[Vertex_Counter+8]=vThree.z;


            Model->Draw_Arrays[n].Texture_Triangle_Coordinates[Texture_Vertex_Counter]=s;
            Model->Draw_Arrays[n].Texture_Triangle_Coordinates[Texture_Vertex_Counter+1]=t;

            Model->Draw_Arrays[n].Texture_Triangle_Coordinates[Texture_Vertex_Counter+2]=sTwo;
            Model->Draw_Arrays[n].Texture_Triangle_Coordinates[Texture_Vertex_Counter+3]=tTwo;

            Model->Draw_Arrays[n].Texture_Triangle_Coordinates[Texture_Vertex_Counter+4]=sThree;
            Model->Draw_Arrays[n].Texture_Triangle_Coordinates[Texture_Vertex_Counter+5]=tThree;


            Model->Draw_Arrays[n].Triangle_Normal_Coordinates[Normal_Counter] = normal.x;
            Model->Draw_Arrays[n].Triangle_Normal_Coordinates[Normal_Counter+1] = normal.y;
            Model->Draw_Arrays[n].Triangle_Normal_Coordinates[Normal_Counter+2] = normal.z;

            Vertex_Counter += 9;
            Normal_Counter += 3;
            Texture_Vertex_Counter += 6;
	    }
    }
}
