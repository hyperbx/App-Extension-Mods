bool m_isStageUpdate = false;

std::string m_lastStageID = "";

void StageListener::Update()
{
	auto currentStageID = StageHelper::GetCurrentStageID();

	if (m_lastStageID != currentStageID)
	{
		m_lastStageID = currentStageID;

		if (m_lastStageID.empty())
			return;

		m_isStageUpdate = true;
	}

	if (!m_isStageUpdate)
		return;

	// The stage has changed during Battle Rush, assume playing all islands.
	if (BattleRushListener::IsBattleRush)
	{
		BattleRushListener::IsBattleRushAll = true;

#if _DEBUG
		printf("[Discord Frontiers] Playing all islands in Battle Rush?\n");
#endif
	}

	Commit(currentStageID);

	m_isStageUpdate = false;
}

void StageListener::Commit(std::string in_stageId)
{
#if _DEBUG
	printf("[Discord Frontiers] World: %s\n", in_stageId.c_str());
#endif

	MasterTrialListener::IsBossRush = false;

	auto pStageData = StageHelper::GetCurrentStageData();

	if (!pStageData)
	{
		printf("[Discord Frontiers] Cannot retrieve info about current stage.\n");
		printf("[Discord Frontiers] Error: app::level::StageData returned nullptr...\n");
		return;
	}

	if (IS_ATTRIBUTE_FLAG(EAttributeFlags_BattleRush))
	{
		BattleRushListener::IsBattleRush = true;
		BattleRushListener::Phase = 0;
		BattleRushListener::PhaseAll = 0;
		return;
	}

	if (IS_ATTRIBUTE_FLAG(EAttributeFlags_BossRush))
	{
		MasterTrialListener::IsBossRush = true;
		MasterTrialListener::CommitBossRush(in_stageId);
		return;
	}
	
	if (IS_ATTRIBUTE_FLAG(EAttributeFlags_Cyber))
	{
		auto name = std::string(pStageData->CyberName.begin(), pStageData->CyberName.end());

		/* This assumes a custom stage has set a unique "cyber_name" string.
		   Otherwise, this will default to regular Cyber Space. */
		bool isCustomStage = name[1] != '-';

		std::string location = isCustomStage
			? "DetailsLocationCustomStage"
			: "DetailsGameModeCyberStage";

		std::string customStageImageUrl = "unknown";

		if (GameModeListener::IsCyberSpaceChallenge)
			location = "DetailsGameModeCyberStageChallenge";

		if (Configuration::URL.empty())
		{
			auto customStageHash = std::to_string(csl::ut::StringMapOperation::hash(name.c_str()));

			customStageImageUrl = std::format(CACHE_URL, customStageHash);

#if _DEBUG
			printf("[Discord Frontiers] Hash: %s\n", customStageHash.c_str());
			printf("[Discord Frontiers] URL:  %s\n", customStageImageUrl.c_str());
#endif

			if (HttpHelper::GetResponse(customStageImageUrl) != 200)
				customStageImageUrl = "unknown";
		}
		else
		{
			printf("[Discord Frontiers] Visualiser URL: %s\n", Configuration::URL.c_str());

			customStageImageUrl = Configuration::URL;

			if (HttpHelper::GetResponse(customStageImageUrl) != 200)
				customStageImageUrl = "missing";
		}

		Discord::Commit
		(
			name,
			LanguageHelper::Localise(location),
			isCustomStage ? customStageImageUrl : StringHelper::ToLower(name),
			PlayerListener::GetCharacterName(),
			
			GameModeListener::IsCyberSpaceChallenge
				? Discord::StartTime
				: TimeHelper::GetSystemEpoch()
		);
	}
	else if (IS_ATTRIBUTE_FLAG(EAttributeFlags_Minigame))
	{
		Discord::CommitState(LanguageHelper::Localise("StateGameModeFishing"));
		Discord::ResetTime();
	}
	else if (IS_ATTRIBUTE_FLAG(EAttributeFlags_Hacking))
	{
		if (IS_ATTRIBUTE_FLAG(EAttributeFlags_LastBoss))
		{
			Discord::Commit(LanguageHelper::Localise("StateBossTheEnd"), "", "w5r01", "supersonic", TimeHelper::GetSystemEpoch());
		}
		else
		{
			Discord::Commit(LanguageHelper::Localise("StateGameModeHacking"), "", "hacking", "sonic", TimeHelper::GetSystemEpoch());
		}
	}
	else if (IS_ATTRIBUTE_FLAG(EAttributeFlags_MasterTrial))
	{
		MasterTrialListener::CommitMasterTrial(in_stageId);
	}
	else if (IS_ATTRIBUTE_FLAG(EAttributeFlags_Tutorial))
	{
		Discord::Commit(LanguageHelper::Localise("StateGameModeTutorial"), "", "training", "sonic", TimeHelper::GetSystemEpoch());
	}
	else
	{
		auto name = GetLocalisedStageID(in_stageId);

		if (name == "unknown")
		{
			Discord::Commit
			(
				"",
				LanguageHelper::Localise("DetailsLocationUnknown"),
				"unknown",
				PlayerListener::GetCharacterName(),
				TimeHelper::GetSystemEpoch()
			);
		}
		else
		{
			Discord::Commit
			(
				Discord::State,
				LanguageHelper::Localise(name),
				StringHelper::ToLower(in_stageId),
				PlayerListener::GetCharacterName(),
				TimeHelper::GetSystemEpoch()
			);
		}
	}
}