//--------------------------------------------------------------------------
/**
 * @file
 * @brief
 * @ingroup Compression
 * @author  N.Ishii
 * @since   Sun May 23 02:50:37 JST 2010
 */
//--------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <yukawa/yukawa.h>
#include <yukawa/mapping48.h>
#include <yukawa/GPM.h>
#include <yukawa/cubic_group.h>
#include <yukawa/4pt.h>
#include <yukawa/misc.h>

Start_Name_Space_Yukawa

/// the destructor
mapping48::~mapping48()
{
  if(rep)   delete[] rep;   rep   = 0;
  if(map)   delete[] map;   map   = 0;
  if(phase) delete[] phase; phase = 0;

  Xsites = Ysites = Zsites = XYZsites = length = 0;
}

/// the default constructor
mapping48::mapping48()
  : Xsites(0),
    Ysites(0),
    Zsites(0),
    XYZsites(0),
    length(0),
    rep(0), map(0), phase(0)
{ cubic_group::initialize(); }

/// the copy constructor
mapping48::mapping48(const mapping48& other)
  : Xsites(other.Xsites),
    Ysites(other.Ysites),
    Zsites(other.Zsites),
    XYZsites(other.XYZsites),
    length(other.length),
    rep(0), map(0), phase(0)
{
  rep   = new int[    length];
  map   = new Intger[ XYZsites];
  phase = new Complex[XYZsites];

  for(int i = 0; i < length;   i++) rep[i]   = other.rep[i];
  for(int i = 0; i < XYZsites; i++) map[i]   = other.map[i];
  for(int i = 0; i < XYZsites; i++) phase[i] = other.phase[i];
}

/// the assignment operator
mapping48& mapping48::operator=(const mapping48& other)
{
  if(rep)   delete[] rep;
  if(map)   delete[] map;
  if(phase) delete[] phase;

  Xsites = other.Xsites;
  Ysites = other.Ysites;
  Zsites = other.Zsites;
 
  XYZsites = other.XYZsites;
  length   = other.length;

  rep   = new int[    length];
  map   = new Intger[ XYZsites];
  phase = new Complex[XYZsites];

  for(int i = 0; i < length;   i++) rep[i]   = other.rep[i];
  for(int i = 0; i < XYZsites; i++) map[i]   = other.map[i];
  for(int i = 0; i < XYZsites; i++) phase[i] = other.phase[i];

  return *this;
}

//--------------------------------------------------------------------------
/**
 * @brief IO (read)
 *
 * The format is <br>
 *
 * int32_t size;                         // size = sizeof(int32_t)*(3 + 1)<br>
 * int32_t Xsites, Ysites, Zsites; <br>
 * int32_t length;                       // length of representative list<br>
 * int32_t size;                         // size = sizeof(int32_t)*(3 + 1)<br>
 *
 * int32_t size;                         // size = sizeof(int32_t)*length<br>
 * int32_t representative_point[length]; // ixyz<br>
 * int32_t size;                         // size = sizeof(int32_t)*length<br>
 * <br>
 *
 * mapping information is constructed after the loading is completed.
 *
 * This is assumed to be written in little endian.
 */
//--------------------------------------------------------------------------

mapping48& mapping48::read(FILE* fp)
{
  if(rep)   delete[] rep;
  if(map)   delete[] map;
  if(phase) delete[] phase;

  bool is_change_endian = is_bigendian();
  int32_t size;
  int32_t tmp[4];

  //
  // reading the size
  //
  if(fread(&size, sizeof(int32_t), 1, fp) != 1){
    fprintf(stderr, "ERROR(mapping48::read) 0\n");
    exit(1);
  }
  if(is_change_endian == true){
    change_endian((uint32_t*)&size, 1);
  }
  if(size != 4 * sizeof(int32_t)){
    fprintf(stderr, "ERROR(mapping48::read) 1\n");
    exit(1);
  }

  //
  // reading the lattice size and the length of the representative list
  //
  if(fread(tmp, sizeof(int32_t), 3, fp) != 3){
    fprintf(stderr, "ERROR(mapping48::read) 2\n");
    exit(1);
  }
  if(is_change_endian == true){
    change_endian((uint32_t*)tmp, 3);
  }
  Xsites = tmp[0];
  Ysites = tmp[1];
  Zsites = tmp[2];

  XYZsites = Xsites * Ysites * Zsites;

  //
  // reading the length of representatives
  //
  if(fread(tmp, sizeof(int32_t), 1, fp) != 1){
    fprintf(stderr, "ERROR(mapping48::read) 3\n");
    exit(1);
  }
  if(is_change_endian == true){
    change_endian((uint32_t*)tmp, 1);
  }
  length = tmp[0];

  rep   = new int[length];

  //
  // reading the size
  //
  if(fread(&size, sizeof(int32_t), 1, fp) != 1){
    fprintf(stderr, "ERROR(mapping48::read) 4\n");
    exit(1);
  }
  if(is_change_endian == true){
    change_endian((uint32_t*)&size, 1);
  }
  if(size != 4 * sizeof(int32_t)){
    fprintf(stderr, "ERROR(mapping48::read) # = 5\n");
    exit(1);
  }




  //
  // reading the size
  //
  if(fread(&size, sizeof(int32_t), 1, fp) != 1){
    fprintf(stderr, "ERROR(mapping48::read) 6\n");
    exit(1);
  }
  if(is_change_endian == true){
    change_endian((uint32_t*)&size, 1);
  }
  if(size != length * (int)sizeof(int32_t)){
    fprintf(stderr, "ERROR(mapping48::read) 7\n");
    exit(1);
  }

  //
  // reading the list of representatives
  //
  for(int i = 0; i < length; i++){
    if(fread(tmp, sizeof(int32_t), 1, fp) != 1){
      fprintf(stderr, "ERROR(mapping48::read) 8\n");
      exit(1);
    }
    if(is_change_endian == true){
      change_endian((uint32_t*)tmp, 1);
    }
    rep[i] = tmp[0];
  }

  //
  // reading the size of the contents (consistency check)
  //
  if(fread(&size, sizeof(int32_t), 1, fp) != 1){
    fprintf(stderr, "ERROR(mapping48::read) 9\n");
    exit(1);
  }
  if(is_change_endian == true){
    change_endian((uint32_t*)&size, 1);
  }
  if(size != length * (int)sizeof(int32_t)){
    fprintf(stderr,
	    "ERROR(mapping48::read): 10\n");
    exit(1);
  }

  map   = new Intger[ XYZsites];
  phase = new Complex[XYZsites];

  return *this;  
}

//--------------------------------------------------------------------------
/**
 * @brief IO (write)
 *
 * This always write in little endian.
 */
//--------------------------------------------------------------------------

const
mapping48& mapping48::write(FILE* fp) const
{
  bool is_change_endian = is_bigendian();

  int32_t size;
  int32_t tmp[4];
  
  //
  // int32_t size = (3 + 1) * sizeof(int32_t);
  //
  size = (3 + 1) * sizeof(int32_t);
  if(is_change_endian == true){
    change_endian((uint32_t*)&size, 1);
  }
  if(fwrite(&size, sizeof(int32_t), 1, fp) != 1){
    fprintf(stderr, "ERROR(mapping48::write) 0\n");
    exit(1);
  }

  //
  // int32_t Xsites, Ysites, Zsites;
  //
  tmp[0] = Xsites;
  tmp[1] = Ysites;
  tmp[2] = Zsites;

  if(is_change_endian == true){
    change_endian((uint32_t*)tmp, 3);
  }
  if(fwrite(tmp, sizeof(int32_t), 3, fp) != 3){
    fprintf(stderr, "ERROR(mapping48::write) 1\n");
    exit(1);
  }

  //
  // int32_t length;
  //
  tmp[0] = length;
  if(is_change_endian == true){
    change_endian((uint32_t*)tmp, 1);
  }
  if(fwrite(tmp, sizeof(int32_t), 1, fp) != 1){
    fprintf(stderr, "ERROR(mapping48::write) 2\n");
      exit(1);
  }

  //
  // int32_t size = (3 + 1) * sizeof(int32_t);
  //
  size = (3 + 1) * sizeof(int32_t);
  if(is_change_endian == true){
    change_endian((uint32_t*)&size, 1);
  }
  if(fwrite(&size, sizeof(int32_t), 1, fp) != 1){
    fprintf(stderr, "ERROR(mapping48::write) 3\n");
    exit(1);
  }



  //
  // int32_t size = length * sizeof(int32_t);
  //
  size = length * sizeof(int32_t);
  if(is_change_endian == true){
    change_endian((uint32_t*)&size, 1);
  }
  if(fwrite(&size, sizeof(int32_t), 1, fp) != 1){
    fprintf(stderr, "ERROR(mapping48::write) 4\n");
    exit(1);
  }

  //
  // int32_t rep[length];
  //
  for(int i = 0; i < length; i++){
    tmp[0] = rep[i];
    if(is_change_endian == true){
      change_endian((uint32_t*)tmp, 1);
    }
    if(fwrite(tmp, sizeof(int32_t), 1, fp) != 1){
      fprintf(stderr, "ERROR(mapping48::write) 5\n");
      exit(1);
    }
  }

  //
  // int32_t size = length * sizeof(int32_t);
  //
  size = length * sizeof(int32_t);
  if(is_change_endian == true){
    change_endian((uint32_t*)&size, 1);
  }
  if(fwrite(&size, sizeof(int32_t), 1, fp) != 1){
    fprintf(stderr, "ERROR(mapping48::write) 4\n");
    exit(1);
  }

  return *this;
}

/// IO (read) [overload]
mapping48& mapping48::read(const char* fname)
{
  bool is_popen = false;
  if(strcmp(&fname[strlen(fname)-3],".gz")==0){
    is_popen = true;
  }

  FILE* fp;
  if(is_popen == true){
    char str[2048];
    sprintf(str, "zcat %s", fname);
    if((fp = popen(str, "r"))==NULL){
      fprintf(stderr, "cannot popen '%s'\n", str);
      exit(1);
    }
  }
  else{
    if((fp = fopen(fname, "r"))==NULL){
      fprintf(stderr, "cannot fopen '%s'\n", fname);
      exit(1);
    }
  }
  read(fp);
  if(is_popen == true){
    pclose(fp);
  }
  else{
    fclose(fp);
  }
  return *this;
}

/// IO (write) [overload]
const
mapping48& mapping48::write(const char* fname) const
{
  bool is_popen = false;
  if(strcmp(&fname[strlen(fname)-3],".gz")==0){
    is_popen = true;
  }

  FILE* fp;
  if(is_popen == true){
    char str[2048];
    sprintf(str, "gzip -c > %s", fname);
    if((fp = popen(str, "w"))==NULL){
      fprintf(stderr, "cannot popen '%s'\n", str);
      exit(1);
    }
  }
  else{
    if((fp = fopen(fname, "w"))==NULL){
      fprintf(stderr, "cannot fopen '%s'\n", fname);
      exit(1);
    }
  }
  write(fp);
  if(is_popen == true){
    pclose(fp);
  }
  else{
    fclose(fp);
  }

  return *this;
}

//--------------------------------------------------------------------------
/**
 *
 *
 */
//--------------------------------------------------------------------------

bool mapping48::is_same_mapping(const mapping48& other) const
{
  if(Xsites != other.Xsites){
    fprintf(stderr,
	    "mapping48: inconsistent Xsites: %d != %d\n",
	    Xsites,
	    other.Xsites);
    exit(1);
  }
  if(Ysites != other.Ysites){
    fprintf(stderr,
	    "mapping48: inconsistent Ysites: %d != %d\n",
	    Ysites,
	    other.Ysites);
    exit(1);
  }
  if(Zsites != other.Zsites){
    fprintf(stderr,
	    "mapping48: inconsistent Zsites: %d != %d\n",
	    Zsites,
	    other.Zsites);
    exit(1);
  }
  if(length != other.length){
    fprintf(stderr,
	    "mapping48: inconsistent length: %d != %d\n",
	    length,
	    other.length);
    exit(1);
  }
  for(int i = 0; i < length; i++){
    if(rep[i] != other.rep[i]){
      fprintf(stderr,
	      "mapping48: inconsistent rep[%d]: %d != %d\n",
	      i,
	      rep[i], other.rep[i]);
      exit(1);
    }
  }
  return true;
}

//--------------------------------------------------------------------------
/**
 * @brief  generate a list of representatives
 * @author N.Ishii
 * @since  Sun May 23 22:06:46 JST 2010
 *
 * The order is such that 0 <= iy <= ix <= iz <= Zsites/2.
 *
 */
//--------------------------------------------------------------------------

mapping48::mapping48(int xsites,int ysites,int zsites)
  : Xsites(xsites),
    Ysites(ysites),
    Zsites(zsites),
    XYZsites(xsites * ysites * zsites),
    length(0),
    rep(0), map(0)
{
  length = 0;
  // 0 <= iy <= ix <= iz <= Zsites/2
  for(int iz = 0; iz <= Zsites/2; iz++)
    for(int ix = 0; ix <= iz; ix++)
      for(int iy = 0; iy <= ix; iy++){
	length++;
      }
  rep = new int[length];

  int index = 0;
    for(int iz = 0; iz <= Zsites/2; iz++)
    for(int ix = 0; ix <= iz; ix++)
      for(int iy = 0; iy <= ix; iy++){
	int ixyz = ix + Xsites*(iy + Ysites*iz);
	rep[index] = ixyz;

	index++;
      }

  map   = new Intger[ XYZsites];
  phase = new Complex[XYZsites];
}

//--------------------------------------------------------------------------
/**
 * @brief  constraction of a mapping between representatives and general points
 *         phase phase
 * @author N.Ishii
 * @since  Sun May 23 22:06:46 JST 2010
 *
 * For the moment, only periodic BC version is implemented.
 */
//--------------------------------------------------------------------------

void mapping48::construct_map_and_phase(BC Xbc, BC Ybc, BC Zbc)
{
  Intger_4pt four(Xsites,Ysites,Zsites);
  Intger_4pt all( Xsites,Ysites,Zsites);
  Intger_4pt tmp( Xsites,Ysites,Zsites);

  int sign = 1;
  for(int index = 0; index < length; index++){
    int tmp  = rep[index];
    int ix = tmp % Xsites; tmp /= Xsites;
    int iy = tmp % Ysites; tmp /= Ysites;
    int iz = tmp % Zsites;
    int ixyz = rep[index];

    four(-ix,-iy,-iz,sign) = Intger(index, 0, -1, ixyz);
    four( ix, iy, iz,sign) = Intger(index, 0,  1, ixyz);
  }

  all = four;

  for(int ith = 1; ith < 24; ith++){
    tmp = rotate(four, ith);
    for(    int iz = 0; iz < Zsites; iz++)
      for(  int iy = 0; iy < Ysites; iy++)
	for(int ix = 0; ix < Xsites; ix++){
	  int ixyz = ix + Xsites*(iy + Ysites*iz);

	  if(tmp[ixyz].index == -1) continue;
	  if(all[ixyz].index == -1){
	    all[ixyz]     = tmp[ixyz];
	    all[ixyz].ith = ith;
	  }
	  else{
	    if(all[ixyz].index != tmp[ixyz].index){
	      fprintf(stderr,
		      "ERROR(mapping48::mapping48()): "
		      "inconsistency is found. (%d,%d,%d) %d != %d\n",
		      ix, iy, iz,
		      all[ixyz].index, tmp[ixyz].index);
	      exit(1);
	    }
	  }
	}
  }

  for(    int iz = 0; iz < Zsites; iz++)
    for(  int iy = 0; iy < Ysites; iy++)
      for(int ix = 0; ix < Xsites; ix++){
	int ixyz = ix + Xsites*(iy + Ysites*iz);
	map[ixyz] = all[ixyz];
	phase[ixyz]     = 1.0;
      }
}

End_Name_Space_Yukawa

