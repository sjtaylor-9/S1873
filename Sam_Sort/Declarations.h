TList *samList, *emmaList, *tigList, *s3List, *s3emmaList, *tigemmaList, *s3tigList, *s3tigemmaList, *tigbgoList, *sbList; 

TList *Egam351keVgateList, *Egam1120keVgateList, *Egam1395keVgateList, *Egam1567keVgateList, *Egam4550keVgateList;

// SSBs
TH1F *h_sb1, *h_sb2;
TH2F *h_sb1_Time, *h_sb2_Time;

// EMMA Singles
TH1F *h_pid;
TH2F *h_icSumVSi, *h_xypos; 

// S3 Singles
TH1F *h_front_back_energy, *h_front_back_energy_w_fractional_cut;
TH2F *h_s3hitmap, *h_s3sectorphi, *h_front_back_v_ring_energy;

// TIGRESS Singles:
TH1F *h_rawE_addback, *h_tigT_bgoT, *h_tigT_bgoT_supp;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TH2F *h_s3energy;
TH2F *h_s3energy_pid_gated;
TH2F * h_s3energy_pid_gated_multi_one;
TH2F * h_s3sector_energy;
TH2F * h_s3ring_energy;
TH2F * h_s3sector_zero_energy_vs_ring_number;
TH1F * h_excite_ring0;
TH1F *h_ejectile_energy_v_thetalab;

// S3-EMMA:
	// PID plots
TH1F *h_excite_pgac_gated, *h_pid_s3emma, *h_excite_pid_gated, *h_S3spec_noMassGate, *h_excite_PIdgated_3538keVgated, *h_excite_PIdgated_5374keVgated, *h_excite_PIdgated_6408keVgated, *h_excite_PIdgated_5902keVgated, *h_excite_PIdgated_4556keVgated, *h_excite_PIdgated_351keVgated, *h_excite_PIdgated_1395keVgated, *h_excite_PIdgated_1745keVgated, *h_excite_PIdgated_1120keVgated, *h_excite_PIdgated_2516keVgated;
TH1F *h_excite_ringA_6408, *h_excite_ringB_6408, *h_excite_ringC_6408, *h_excite_ringD_6408, *h_excite_ringE_6408, *h_excite_ringF_6408;
TH1F *h_excite_ringA_4556, *h_excite_ringB_4556, *h_excite_ringC_4556, *h_excite_ringD_4556, *h_excite_ringE_4556, *h_excite_ringF_4556;
TH2F *h_icSumVSi_s3emma, *h_gammagamma;
	// Theta for each state
TH1F *h_theta_Ex1745keV, *h_theta_Ex2866keV, *h_theta_Ex3735keV, *h_theta_Ex4433keV, *h_theta_Ex5334keV, *h_theta_Ex7420keV, *h_theta_Ex8155keV;
	// Kinematics
TH2F *h_energy_v_lab_ang_gated, *h_sector_v_excite_gated, *h_phi_v_excite_gated, *h_theta_v_excite_gated;
	// Theta v Excite for Each S3 Quadrant
TH2F *h_theta_v_excite_q1, *h_theta_v_excite_q2, *h_theta_v_excite_q3, *h_theta_v_excite_q4, *s3_E_theta;
	// Recoil Angles
TH1F *h_recoil_vang, *h_recoil_hang;
TH2F *h_recoil_ang;
TH1F *h_recoil_vang_Ex7420keV, *h_recoil_hang_Ex7420keV;
TH2F *h_recoil_ang_Ex7420keV;
TH1F *h_excite_lowrecoilang;
TH2F *h_excite_recoilang;
TH2F *h_theta_recoil_v_theta_triton_gated;
TH2F *h_recoil_ang_s18;
	// Theta vs Phi
TH1F *h_theta, *h_phi, *h_recoil_theta, *h_recoil_phi;
TH2F *h_theta_phi, *h_recoil_theta_phi;
	// S3 Hit Map
TH2F *h_s3emmahitmap;

// Excite vs Ring:
TH2F *h_S3EMMA_Exc_RngN;
// Excite vs Sector
TH2F *h_S3EMMA_Exc_SecN;

// Fluorine Gated
TH2F *F_excite_v_dopE_addback ;
TH1F *F_dopE_addback_gated, *F_excite_pid_gated;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// S3-TIG-EMMA:
TH1F *h_excite_gamma_gate, *h_lab_ang_gamma_gate, *h_dopE_addback_gated, *h_dopE_addback_reverseGate_higher, *h_dopE_addback_reverseGate_lower, *h_dopE_addback_reverseGate_middle, *h_dopE_addback_fluorine, *h_lab_ang_Ex7420keV, *h_s3emmatof, *h_tigemmatof, *h_lab_ang_Ex8155keV, *h_lab_ang_Ex4433keV, *h_excite_pid_gated_DL_corr, *h_excite_pid_gated_DL_tLiF_corr;
TH2F *h_excite_v_dopE_addback, *h_excite_v_dopE_addback_fluorine, *h_addE_tof, *h_icSumVSi_gated;
TH1F *h_s3_E, *h_s3_E_DL_corr, *h_s3_E_DL_tLiF_corr;
TH2F *h_excite_s3_E;
TH2F *h_sectorE_v_ring_num;
TH2F *h_phi_excite_351gate, *h_phi_excite_1120gate, *h_phi_excite_1395gate, *h_phi_excite_1567gate, *h_phi_excite_4550gate;
TH2F *h_theta_excite_351gate, *h_theta_excite_1120gate, *h_theta_excite_1395gate, *h_theta_excite_1567gate, *h_theta_excite_4550gate;
TH1F *h_excite_351gate, *h_excite_1120gate, *h_excite_1395gate, *h_excite_1567gate, *h_excite_4550gate;
TH1F *h_theta_351gate, *h_theta_1120gate, *h_theta_1395gate, *h_theta_1567gate, *h_theta_4550gate;

TH1F *h_pgac_hits;
// 3D histograms
TH3F *h_S3TIGEMMA_Exc_EdopAdd_RngN;
//TH3F *h_S3TIGEMMA_Exc_EdopSng_RngN;

//TCuts
TCutG *Neon_Gate, *Fluorine_Gate;
