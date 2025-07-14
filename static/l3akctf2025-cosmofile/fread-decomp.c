size_t __fastcall fread_unlocked(void *buf, size_t stride, size_t count, FILE *f)
{
  unsigned int beg; // ecx
  unsigned int end; // esi
  unsigned __int128 v9; // rax
  size_t v11; // r9
  size_t v12; // r14
  size_t v13; // r15
  int fd; // eax
  size_t v15; // r8
  size_t size; // rdx
  char *v17; // rcx
  int v18; // ecx
  iovec *v19; // r9
  unsigned __int64 v20; // rax
  size_t v21; // rdx
  int v22; // esi
  unsigned __int64 v23; // rcx
  int v24; // ecx
  errno_t *v26; // rdx
  int v27; // eax
  errno_t *v28; // rdx
  int v29; // eax
  errno_t *v30; // rdx
  int v31; // eax
  unsigned int v32; // r9d
  size_t v33; // r12
  unsigned int v34; // ecx
  size_t v35; // [rsp+8h] [rbp-68h]
  int v36; // [rsp+14h] [rbp-5Ch]
  size_t n; // [rsp+18h] [rbp-58h]
  int nb; // [rsp+18h] [rbp-58h]
  size_t na; // [rsp+18h] [rbp-58h]
  iovec iov[2]; // [rsp+20h] [rbp-50h] BYREF

  if ( (f->oflags & 3) == 1 )
  {
    v28 = _errno_location();
    v29 = EBADF;
    *v28 = EBADF;
    f->state = v29;
    return 0LL;
  }
  beg = f->beg;
  end = f->end;
  if ( end < beg )
  {
    v26 = _errno_location();
    v27 = EINVAL;
    *v26 = EINVAL;
    f->state = v27;
    return 0LL;
  }
  v9 = count * (unsigned __int128)stride;
  v11 = v9;
  v12 = *((_QWORD *)&v9 + 1) != 0LL;
  if ( !is_mul_ok(count, stride) )
  {
    v30 = _errno_location();
    v31 = EOVERFLOW;
    *v30 = EOVERFLOW;
    f->state = v31;
    return 0LL;
  }
  if ( !(_QWORD)v9 )
    return 0LL;
  v13 = end - beg;
  if ( v13 >= (unsigned __int64)v9 )
  {
    nb = v9;
    memmove(buf, &f->buf[beg], v9);
    v32 = f->beg + nb;
    f->beg = v32;
    if ( v32 == f->end )
      *(_QWORD *)&f->beg = 0LL;
    return count;
  }
  fd = f->fd;
  if ( fd == -1 )
  {
    v33 = stride * (v13 / stride);
    if ( v33 )
    {
      memmove(buf, &f->buf[beg], stride * (v13 / stride));
      beg = f->beg;
      end = f->end;
    }
    v34 = v33 + beg;
    f->beg = v34;
    if ( v34 == end )
    {
      f->state = -1;
      *(_QWORD *)&f->beg = 0LL;
    }
    return v33 / stride;
  }
  if ( end != beg )
  {
    na = v11;
    buf = memmove(buf, &f->buf[beg], v13);
    fd = f->fd;
    v11 = na;
  }
  iov[0].iov_base = (char *)buf + v13;
  v15 = v11 - v13;
  iov[0].iov_len = v11 - v13;
  if ( f->bufmode == 2 || (size = f->size, v11 >= size) )
  {
    size = 0LL;
    v17 = 0LL;
  }
  else
  {
    v17 = f->buf;
    if ( (unsigned int)size > 0xC )
      size = (unsigned int)(size - 12);
  }
  iov[1].iov_base = v17;
  iov[1].iov_len = size;
  v18 = 2;
  v19 = iov;
  while ( 2 )
  {
    v35 = v15;
    v36 = v18;
    n = (size_t)v19;
    v20 = readv(fd, v19, v18);
    v15 = v35;
    if ( v20 == -1LL )
    {
      f->state = *_errno_location();
      v15 = v35;
      if ( v12 )
        goto LABEL_21;
      return 0LL;
    }
    v12 += v20;
    if ( !v20 )
    {
      f->state = -1;
      goto LABEL_21;
    }
    v21 = n + 16;
    v22 = v36 - 1;
    while ( 1 )
    {
      v23 = *(_QWORD *)(v21 - 8);
      if ( v23 )
        break;
LABEL_19:
      v24 = v22;
      v21 += 16LL;
      --v22;
      if ( !v24 )
        goto LABEL_20;
    }
    if ( v20 >= v23 )
    {
      v20 -= v23;
      goto LABEL_19;
    }
    v19 = (iovec *)(v21 - 16);
    *(_QWORD *)(v21 - 16) += v20;
    *(_QWORD *)(v21 - 16 + 8) = v23 - v20;
    if ( v12 < v35 )
    {
      v18 = v22 + 1;
      fd = f->fd;
      continue;
    }
    break;
  }
LABEL_20:
  if ( v12 == -1LL )
    return 0LL;
LABEL_21:
  if ( v12 >= v15 )
  {
    f->beg = 0;
    f->end = v12 - v15;
    return count;
  }
  *(_QWORD *)&f->beg = 0LL;
  return (v13 + v12) / stride;
}
