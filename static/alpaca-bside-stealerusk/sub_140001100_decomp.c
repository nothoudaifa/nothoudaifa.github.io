__int64 __fastcall sub_140001100(UCHAR *a1, ULONG a2, UCHAR *a3, UCHAR **a4, _DWORD *a5)
{
  size_t v5; // rax
  UCHAR *Block; // [rsp+58h] [rbp-80h]
  PUCHAR pbIV; // [rsp+60h] [rbp-78h]
  unsigned int v9; // [rsp+68h] [rbp-70h]
  UCHAR *pbKeyObject; // [rsp+70h] [rbp-68h]
  UCHAR v11[4]; // [rsp+78h] [rbp-60h] BYREF
  ULONG cbOutput; // [rsp+7Ch] [rbp-5Ch] BYREF
  UCHAR pbOutput[4]; // [rsp+80h] [rbp-58h] BYREF
  BCRYPT_ALG_HANDLE phAlgorithm; // [rsp+88h] [rbp-50h] BYREF
  BCRYPT_KEY_HANDLE phKey; // [rsp+90h] [rbp-48h] BYREF
  ULONG pcbResult; // [rsp+98h] [rbp-40h] BYREF
  UCHAR pbBuffer[16]; // [rsp+A0h] [rbp-38h] BYREF

  phAlgorithm = 0;
  phKey = 0;
  pcbResult = 0;
  *(_DWORD *)pbOutput = 0;
  *(_DWORD *)v11 = 0;
  pbKeyObject = 0;
  pbIV = 0;
  Block = 0;
  cbOutput = 0;
  v9 = 0;
  if ( BCryptGenRandom(0, pbBuffer, 0x10u, 2u) >= 0
    && BCryptOpenAlgorithmProvider(&phAlgorithm, L"AES", 0, 0) >= 0
    && BCryptSetProperty(phAlgorithm, L"ChainingMode", (PUCHAR)L"ChainingModeCBC", 0x20u, 0) >= 0
    && BCryptGetProperty(phAlgorithm, L"ObjectLength", pbOutput, 4u, &pcbResult, 0) >= 0
    && BCryptGetProperty(phAlgorithm, L"BlockLength", v11, 4u, &pcbResult, 0) >= 0
    && *(_DWORD *)v11 == 16 )
  {
    pbKeyObject = (UCHAR *)malloc(*(unsigned int *)pbOutput);
    if ( pbKeyObject )
    {
      pbIV = (PUCHAR)malloc(*(unsigned int *)v11);
      if ( pbIV )
      {
        qmemcpy(pbIV, pbBuffer, *(unsigned int *)v11);
        if ( BCryptGenerateSymmetricKey(phAlgorithm, &phKey, pbKeyObject, *(ULONG *)pbOutput, a3, 0x10u, 0) >= 0
          && BCryptEncrypt(phKey, a1, a2, 0, pbIV, *(ULONG *)v11, 0, 0, &cbOutput, 1u) >= 0 )
        {
          v5 = cbOutput + 16;
          if ( cbOutput >= 0xFFFFFFF0 )
            v5 = -1;
          Block = (UCHAR *)malloc(v5);
          if ( Block )
          {
            qmemcpy(pbIV, pbBuffer, *(unsigned int *)v11);
            if ( BCryptEncrypt(phKey, a1, a2, 0, pbIV, *(ULONG *)v11, Block + 16, cbOutput, &cbOutput, 1u) >= 0 )
            {
              qmemcpy(Block, pbBuffer, *(unsigned int *)v11);
              *a4 = Block;
              *a5 = cbOutput + 16;
              Block = 0;
              v9 = 1;
            }
          }
        }
      }
    }
  }
  if ( Block )
    free(Block);
  if ( pbIV )
    free(pbIV);
  if ( pbKeyObject )
    free(pbKeyObject);
  if ( phKey )
    BCryptDestroyKey(phKey);
  if ( phAlgorithm )
    BCryptCloseAlgorithmProvider(phAlgorithm, 0);
  return v9;
}
