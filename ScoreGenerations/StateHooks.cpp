const char* StateHooks::stageID = (const char*)0x1E774D4;

#pragma region ----- Hooked Functions -----

void OnLoad()
{
	// Disable the score counter for forbidden stages.
	if (HudSonicStage::IsStageForbidden())
	{
		// Use Sonic Generations' HUD XNCP.
		if (!Configuration::customXNCP)
			WRITE_MEMORY(0x168E333, uint8_t, "_default\0");

		// Skip Casino Night score instructions.
		if (!HudSonicStage::IsCasino())
			WRITE_MEMORY(0x109C1DA, uint8_t, 0xEB, 0x78);

		// Disable results hooks.
		StateHooks::HookResults(false);
	}
	else
	{
		if (!Configuration::customXNCP)
		{
			if (StringHelper::GetDigits(Configuration::scoreFormat) == 6)
			{
				// Use Score Generations' HUD XNCP for six digit padding.
				WRITE_MEMORY(0x168E333, uint8_t, "_low_padding\0");
			}
			else
			{
				// Use Score Generations' HUD XNCP for high amounts of padding.
				WRITE_MEMORY(0x168E333, uint8_t, "_high_padding\0");
			}
		}

		// Execute Casino Night score instructions.
		WRITE_MEMORY(0x109C1DA, uint8_t, 0xEB, 0x00);

		// Hook to results for local score.
		StateHooks::HookResults(true);
	}
}

#pragma endregion

#pragma region ----- Mid-ASM Hooks -----

__declspec(naked) void Loading_MidAsmHook()
{
	static void* interruptAddress = (void*)0x65FCC0;
	static void* returnAddress = (void*)0x448E98;

	// Reset statistics.
	ScoreListener::Reset();

	__asm
	{
		// Perform actions on the loading screen.
		call OnLoad

		call [interruptAddress]

		jmp [returnAddress]
	}
}

__declspec(naked) void ClassicSonicPrepareRestart_MidAsmHook()
{
	static void* interruptAddress = (void*)0xDD6740;
	static void* returnAddress = (void*)0xDEB841;

	__asm
	{
		call [interruptAddress]

		// Reset statistics.
		call ScoreListener::Reset

		jmp [returnAddress]
	}
}

__declspec(naked) void ModernSonicPrepareRestart_MidAsmHook()
{
	static void* interruptAddress = (void*)0xE14350;
	static void* returnAddress = (void*)0xE28C7B;

	__asm
	{
		call [interruptAddress]

		// Reset statistics.
		call ScoreListener::Reset

		jmp [returnAddress]
	}
}

__declspec(naked) void Exit_MidAsmHook()
{
	static void* interruptAddress = (void*)0x6AE910;
	static void* returnAddress = (void*)0x42AD76;

	__asm
	{
		call [interruptAddress]

		// Reset statistics.
		call ScoreListener::Reset

		jmp [returnAddress]
	}
}

__declspec(naked) void ResultsCalculate_MidAsmHook()
{
	static void* returnAddress = (void*)0xD5A191;

	// Gather the results.
	ResultListener::Result();

	// Reset statistics.
	ScoreListener::Reset();

	__asm
	{
		lea eax, ResultListener::resultDescription

		jmp [returnAddress]
	}
}

#pragma endregion

void StateHooks::HookResults(bool enabled)
{
	if (enabled)
	{
		// Calculate results with local statistics.
		WRITE_JUMP(0xD5A18C, &ResultsCalculate_MidAsmHook);
	}
	else
	{
		// Restore default results calculation.
		WRITE_MEMORY(0xD5A18C, uint8_t, 0xE8, 0x1F, 0x9C, 0x35, 0x00);
	}
}

void StateHooks::Install()
{
	// Update local loading function.
	WRITE_JUMP(0x448E93, &Loading_MidAsmHook)

	// Reset statistics upon restarting.
	WRITE_JUMP(0xDEB83C, &ClassicSonicPrepareRestart_MidAsmHook);
	WRITE_JUMP(0xE28C76, &ModernSonicPrepareRestart_MidAsmHook);

	// Reset statistics upon exiting.
	WRITE_JUMP(0x42AD71, &Exit_MidAsmHook);
}