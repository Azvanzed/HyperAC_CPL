#include "memory.h"
#include "common.h"
#include "config.h"
#include <intrin.h>

NTSTATUS SafeVirtualCopy(_Out_ PVOID Dst, _In_ CONST PVOID Src, _In_ SIZE_T Size)
{
	

	SIZE_T copied_bytes = 0;
	NTSTATUS status = STATUS_SUCCESS;

	if (Dst == NULL || Src == NULL || Size == 0)
	{
		return FALSE;
	}

	status = MmCopyMemory(Dst, *(MM_COPY_ADDRESS*)&Src, Size, MM_COPY_MEMORY_VIRTUAL, &copied_bytes);
	if (copied_bytes != Size)
	{
		return STATUS_COPY_PROTECTION_FAILURE;
	}

	return status;
}

NTSTATUS SafePhysicalCopy(_Out_ PVOID Dst, _In_ CONST PVOID Src, _In_ SIZE_T Size)
{
	

	SIZE_T copied_bytes = 0;
	NTSTATUS status = STATUS_SUCCESS;

	if (Dst == NULL || Src == NULL || Size == 0)
	{
		return FALSE;
	}

	status = MmCopyMemory(Dst, *(MM_COPY_ADDRESS*)&Src, Size, MM_COPY_MEMORY_PHYSICAL, &copied_bytes);
	if (copied_bytes != Size)
	{
		return STATUS_COPY_PROTECTION_FAILURE;
	}

	return status;
}

ULONG64 FindSignature(_In_ UCHAR* Data, _In_ SIZE_T Size, _In_ CONST CHAR* Pattern, _In_ CONST CHAR* Mask)
{
	

	SIZE_T length = 0;
	
	length = strlen(Mask);

	for (SIZE_T i = 0; i < Size; ++i)
	{
		for (SIZE_T j = 0; j < length; ++j)
		{
			if (Mask[j] != '?' && Data[i + j] != (UCHAR)Pattern[j])
			{
				break;
			}

			if (j == length - 1)
			{
				return (ULONG64)Data + i + j;
			}
		}
	}

	return 0;
}

ULONG64 ToVirtual(_In_ LONG64 PhysicalAddress)
{
	

	PVOID va = NULL;

	va = MmGetVirtualForPhysical(*(PHYSICAL_ADDRESS*)&PhysicalAddress);
	return va;
}

LONG64 ToPhysical(_In_ ULONG64 VirtualAddress)
{
	

	PHYSICAL_ADDRESS pa = { 0 };

	pa = MmGetPhysicalAddress((PVOID)VirtualAddress);
	return pa.QuadPart;
}