#include "types.hpp"
#include "utils.hpp"

/*
 * Advanced Configuration and Power Interface (ACPI) module
 * ACPI specifications available at https://www.uefi.org/acpi/specs
 */

/*
 * This is a structure for the Root System Description Pointer (RSDP).
 * The main reason for this is to be able to find the Root System
 * Description Table (RSDT).
 */
struct rsdp
{
  char signature[8];
  uint8_t checksum;
  char oemid[6];
  uint8_t revision;
  uint32_t rsdt_addr;
} __attribute__ ((packed));

/*
 * Other than the RSDT, regular System Description Tables (SDTs) use
 * the following layout in memory for their header.
 */
struct sdt_hdr
{
  char signature[4];
  uint32_t length;
  uint8_t revision;
  uint8_t checksum;
  char oemid[6];
  char oem_tableid[8];
  uint32_t oem_revision;
  uint32_t creator_id;
  uint32_t creator_revision;
  /* SDT specific data follows */
} __attribute__ ((packed));

/*
 * This is the memory layout for the Multiple APIC Description Table (MADT).
 * Where APIC stands for Advanced Programmable Interrupt Controller.
 */
struct apic_sdt
{
  struct sdt_hdr header;
  uint32_t lapic_addr;
  uint32_t flags;
  /*
   * Here we will have a variable sized area with additional fields
   * up to header.length bytes.
   */
} __attribute__ ((packed));

/* Returns a pointer to the RSDP */
struct rsdp *find_rsdp()
{
  uint32_t i = 0;
  for (i = 0x000E0000; i < 0x000FFFFF; i += 16)
  {
    const unsigned char *mem = (const unsigned char *) i;
    if (memcmp(mem, "RSD PTR ", 8) == 0) {
      //printf("Found RSDP at address 0x%x\n", i);
      return (struct rsdp *)i;
    }
  }
  return 0;
}

int validate_rsdp(struct rsdp *rsdp)
{
  /* Check we recognize the ACPI revision number */
  if (rsdp->revision == 0) {
    printf("ACPI Version 1.0\n");
  } else {
    printf("Unknown ACPI Version. Revision: %d\n", rsdp->revision);
    return 0;
  }

  /* Last byte of the sum of all bytes in RSDP should be zero */
  uint8_t total_bytes = 0;
  unsigned int i = 0;
  for (i = 0; i < sizeof(struct rsdp); i++) {
    total_bytes += *(((uint8_t *) rsdp) + i);
  }

  return total_bytes == 0;
}

int validate_sdt(struct sdt_hdr *sdt)
{
  /* 8-bit checksum, all bytes summed should be zero */
  uint8_t total_bytes = 0;
  unsigned int i = 0;
  for (i = 0; i < sdt->length; i++) {
    total_bytes += *(((uint8_t *) sdt) + i);
  }

  return total_bytes == 0;
}

void print_signature(char *signature, int length)
{
  int i = 0;
  for (i = 0; i < length; i++) {
    printf("%c", signature[i]);
  }
}

void init_acpi()
{
  struct rsdp *rsdp = find_rsdp();
  if (rsdp == 0) {
    printf("RSDP not found\n");
    return;
  }

  if (!validate_rsdp(rsdp)) {
    printf("Invalid RSDP detected\n");
    return;
  }

  struct sdt_hdr *rsdt = (struct sdt_hdr *) rsdp->rsdt_addr;
  if (!validate_sdt(rsdt)) {
    printf("Invalid RSDT detected\n");
    return;
  }

  /*
   * The Root System Description Table (RSDT) will contain 32-bit pointers
   * to all the other SDT tables. The number of SDT tables pointed to
   * by the RSDT is the length in bytes of the RSDT (which includes the
   * header) divided by 4 (since each pointer to an SDT is 32 bits).
   */
  int num_sdt = (rsdt->length - sizeof(struct sdt_hdr)) / 4;
  printf("Number of SDT tables: %d\n", num_sdt);
  uint32_t *ptrs_other_sdt = (uint32_t *)((uint8_t *)rsdt + sizeof(struct sdt_hdr));
  struct apic_sdt *apic_sdt = 0;
  int i = 0;
  for (i = 0; i < num_sdt; i++) {
    struct sdt_hdr *sdt = (struct sdt_hdr *) *(ptrs_other_sdt + i);
    print_signature(sdt->signature, sizeof(sdt->signature));
    if (memcmp(sdt->signature, "APIC", 4) == 0) {
      apic_sdt = (struct apic_sdt *) sdt;
    }
    printf(" ");
  }
  printf("\n");

  /* Validate APIC SDT before using it */
  if (!validate_sdt(&apic_sdt->header)) {
    printf("Invalid APIC SDT detected\n");
    return;
  }

  /* Detect number of CPUs */
  uint8_t *p = (uint8_t *)apic_sdt + sizeof(struct apic_sdt);
  uint8_t *pmax = (uint8_t *)apic_sdt + apic_sdt->header.length;
  int num_cpus = 0;
  for (; p < pmax; p += p[1]) {
    int type = p[0];
    switch(type) {
      case 0: num_cpus++; break;
      /* TODO Handle more types here */
    }
  }
  printf("Detected %d CPUs\n", num_cpus);
}
