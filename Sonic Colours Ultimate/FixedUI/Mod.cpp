void* CLevelInfo;

// 0x1507D0A60
SIG_SCAN(sigCLevelInfoCtor, "\x40\x53\x48\x83\xEC\x20\x45\x31\xC0\x48\x89\xCB\x41\x8D\x50\x03\xE8\x00\x00\x00\x00\x31\xC9\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x03\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x43\x18\x31\xC0\x48\x89\x8B\x00\x00\x00\x00\x89\x8B\x00\x00\x00\x00\x48\x89\x8B\x00\x00\x00\x00\x48\x89\x8B\x00\x00\x00\x00\x89\x8B\x00\x00\x00\x00\x66\x89\x8B\x00\x00\x00\x00\x88\x8B\x00\x00\x00\x00\x48\x89\x43\x68\x48\x89\x43\x70\x48\x89\x43\x78\x48\x89\x83\x00\x00\x00\x00\x48\x89\x83\x00\x00\x00\x00\x48\x89\x83\x00\x00\x00\x00\x48\x89\x83\x00\x00\x00\x00\x48\x89\x83\x00\x00\x00\x00\x48\x89\x83\x00\x00\x00\x00\x48\x89\xD8\xC7\x43\x00\x00\x00\x00\x00\x89\x4B\x6C\xC7\x83\x00\x00\x00\x00\x00\x00\x00\x00\x89\x8B\x00\x00\x00\x00\x48\x83\xC4\x20\x5B\xC3", "xxxxxxxxxxxxxxxxx????xxxxx????xxxxxx????xxxxxxxxx????xx????xxx????xxx????xx????xxx????xx????xxxxxxxxxxxxxxx????xxx????xxx????xxx????xxx????xxx????xxxxx?????xxxxx????????xx????xxxxxx");
HOOK(void, __fastcall, CLevelInfoCtor, sigCLevelInfoCtor(), void* a1)
{
	CLevelInfo = a1;

	originalCLevelInfoCtor(a1);
}

// 0x1507D36D0
SIG_SCAN(sigCLevelInfoDtor, "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x20\x48\x8D\x05\x00\x00\x00\x00\x48\x89\xCF\x48\x89\x01\x89\xD3\x48\x83\xC1\x18\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x01\xE8\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x07\xF6\xC3\x01\x74\x0D\xBA\x00\x00\x00\x00\x48\x89\xF9\xE8\x00\x00\x00\x00\x48\x8B\x5C\x24\x00\x48\x89\xF8\x48\x83\xC4\x20\x5F\xC3", "xxxx?xxxxxxxx????xxxxxxxxxxxxxxx????xxxx????xxx????xxxxxxxxx????xxxx????xxxx?xxxxxxxxx");
HOOK(void, __fastcall, CLevelInfoDtor, sigCLevelInfoDtor(), void* a1, char a2)
{
	CLevelInfo = nullptr;

	originalCLevelInfoDtor(a1, a2);
}

// 0x1416DDA90
SIG_SCAN(sigReturnToTitle, "\x40\x53\x48\x83\xEC\x40\x48\xC7\x44\x24\x00\x00\x00\x00\x00\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x90\xE8\x00\x00\x00\x00\x48\x85\xC0\x74\x4B\x48\x8B\xC8\xE8\x00\x00\x00\x00\x48\x85\xC0\x74\x3E\x8B\x58\x08\xE8\x00\x00\x00\x00\x48\x85\xC0\x74\x20\x48\x8B\xC8\xE8\x00\x00\x00\x00\x48\x85\xC0\x74\x13\x48\x8B\xC8\xE8\x00\x00\x00\x00\x48\x85\xC0\x74\x06\x48\x8B\x48\x20\xEB\x02\x33\xC9\x89\x5C\x24\x38\x48\x8D\x54\x24\x00\xE8\x00\x00\x00\x00\x90\x48\x83\xC4\x40\x5B\xC3", "xxxxxxxxxx?????xxxx?x????xx????xxxxxxxxx????xxxxxxxxx????xxxxxxxxx????xxxxxxxxx????xxxxxxxxxxxxxxxxxxxxx?x????xxxxxxx");
FUNCTION_PTR(void, __cdecl, ReturnToTitle, sigReturnToTitle());

// 0x141753250
SIG_SCAN(sigMsgPLNotifyDeadCtor, "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x20\x8B\xDA\x48\x8B\xF9\xBA\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x89\x5F\x18\x48\x8B\x5C\x24\x00\x48\x89\x07\x48\x8B\xC7\x48\x83\xC4\x20\x5F\xC3", "xxxx?xxxxxxxxxxx????x????xxx????xxxxxxx?xxxxxxxxxxxx");
HOOK(void, __fastcall, MsgPLNotifyDeadCtor, sigMsgPLNotifyDeadCtor(), void* a1)
{
	int* lives = (int*)((char*)CLevelInfo + 112);

	// Clamp lives to zero to prevent it falling into negatives.
	if ((*lives - 1) != 0)
		--*lives;
	else
		ReturnToTitle();

	originalMsgPLNotifyDeadCtor(a1);
}

// 0x15089BB00
SIG_SCAN(sigMsgPlayerPassTailsSaveMarkerCtor, "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x30\x48\x89\xD3\x0F\x29\x74\x24\x00\x8B\x15\x00\x00\x00\x00\x81\xF2\x00\x00\x00\x00\x41\x0F\xB6\xF9\x0F\x28\xF2\x48\x89\xCE\xE8\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x06\x8B\x03\x89\x46\x18\x8B\x43\x04\x89\x46\x1C\x8B\x43\x08\x48\x8B\x5C\x24\x00\x89\x46\x20\x0F\xB6\x44\x24\x00\x88\x46\x29\x8B\x44\x24\x68\x89\x46\x2C\x48\x89\xF0\xF3\x0F\x11\x76\x00\x0F\x28\x74\x24\x00\x40\x88\x7E\x28\x48\x8B\x74\x24\x00\x48\x83\xC4\x30\x5F\xC3", "xxxx?xxxx?xxxxxxxxxxxx?xx????xx????xxxxxxxxxxx????xxx????xxxxxxxxxxxxxxxxxxxxx?xxxxxxx?xxxxxxxxxxxxxxxxx?xxxx?xxxxxxxx?xxxxxx");
HOOK(void, __fastcall, MsgPlayerPassTailsSaveMarkerCtor, sigMsgPlayerPassTailsSaveMarkerCtor(), __int64 a1, DWORD* a2, float a3, char a4, char a5, int a6)
{
	return;
}

/// <summary>
/// The main entry point for the injection.
/// </summary>
extern "C" _declspec(dllexport) void Init()
{
	Configuration::Read();

	if (!StringHelper::Compare(Configuration::tailsSave, "."))
	{
		// Skip MsgPLNotifyTailsSave.
		WRITE_JUMP(0x1418B13B4, (void*)0x1418B13BD);

		INSTALL_HOOK(CLevelInfoCtor);
		INSTALL_HOOK(CLevelInfoDtor);
		INSTALL_HOOK(MsgPLNotifyDeadCtor);
		INSTALL_HOOK(MsgPlayerPassTailsSaveMarkerCtor);
	}
}