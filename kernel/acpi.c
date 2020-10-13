#include "types.h"
#include "utils.h"

struct rsdp
{
  char signature[8];
  uint8_t checksum;
  char oemid[6];
  uint8_t revision;
  uint32_t rsdt_addr;
} __attribute__ ((packed));

struct sdt_hdr {
  char signature[4];
  uint32_t length;
  uint8_t revision;
  uint8_t checksum;
  char oemid[6];
  char oem_tableid[8];
  uint32_t oem_revision;
  uint32_t creator_id;
  uint32_t creator_revision;
} __attribute__ ((packed));

/* Returns a pointer to the RSDP */
struct rsdp *find_rsdp()
{
  uint32_t i = 0;
  for (i = 0x000E0000; i < 0x000FFFFF; i += 16)
  {
    const unsigned char *mem = (const unsigned char *) i;
    if (memcmp(mem, (const unsigned char *) "RSD PTR ", 8) == 0) {
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
  int i = 0;
  for (i = 0; i < sizeof(struct rsdp); i++) {
    total_bytes += *(((uint8_t *) rsdp) + i);
  }

  return total_bytes == 0;
}

int validate_sdt(struct sdt_hdr *sdt)
{
  /* 8-bit checksum, all bytes summed should be zero */
  uint8_t total_bytes = 0;
  int i = 0;
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

  int rsdp_valid = validate_rsdp(rsdp);
  if (!rsdp_valid) {
    printf("Invalid RSDP detected\n");
    return;
  }

  struct sdt_hdr *rsdt = (struct sdt_hdr *) rsdp->rsdt_addr;
  int rsdt_valid = validate_sdt(rsdt);
  if (!rsdt_valid) {
    printf("Invalid RSDT detected\n");
    return;
  } else {
    print_signature(rsdt->signature, sizeof(rsdt->signature));
    printf("\n");
  }

  /*
   * The Root System Descriptor Table (RSDT) will contain 32-bit pointers
   * to all the other SDT tables. The number of SDT tables pointed to
   * by the RSDT is the length in bytes of the RSDT (which includes the
   * header) divided by 4 (since each pointer to an SDT is 32 bits).
   */
  int num_sdt = (rsdt->length - sizeof(struct sdt_hdr)) / 4;
  printf("Number of SDT tables: %d\n", num_sdt);
  uint32_t *ptrs_other_sdt = (uint32_t *)((void *)rsdt + sizeof(struct sdt_hdr));
  int i = 0;
  for (i = 0; i < num_sdt; i++) {
    struct sdt_hdr *sdt = (struct sdt_hdr *) *(ptrs_other_sdt + i);
    print_signature(sdt->signature, sizeof(sdt->signature));
    printf(" ");
  }
  printf("\n");
}
