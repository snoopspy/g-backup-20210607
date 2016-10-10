#include "gdns.h"

// ----------------------------------------------------------------------------
// SnoopDnsQuestion
// ----------------------------------------------------------------------------
QByteArray GDns::SnoopDnsQuestion::encode() {
  QByteArray res;

  res = GDns::encodeName(this->name);

  uint16_t _type = htons(this->type);
  res.append((const char*)&_type, sizeof(uint16_t));

  uint16_t _class_ = htons(this->class_);
  res.append((const char*)&_class_, sizeof(uint16_t));

  return res;
}

bool GDns::SnoopDnsQuestion::decode(u_char* udpData, size_t dataLen, size_t* offset)
{
  this->name = GDns::decodeName(udpData, dataLen, offset);
  if (this->name == "") return false;

  if (*offset + sizeof(uint16_t) > dataLen) return false;
  uint16_t* _type = (uint16_t*)(udpData + *offset);
  this->type = ntohs(*_type);
  *offset += sizeof(uint16_t);

  if (*offset + sizeof(uint16_t) > dataLen) return false;
  uint16_t* _class_ = (uint16_t*)(udpData + *offset);
  this->class_ = ntohs(*_class_);
  *offset += sizeof(uint16_t);

  if (*offset > dataLen) return false;
  return true;
}

// ----------------------------------------------------------------------------
// SnoopDnsQuestions
// ----------------------------------------------------------------------------
QByteArray GDns::SnoopDnsQuestions::encode()
{
  QByteArray res;
  foreach (SnoopDnsQuestion question, *this)
  {
    res += question.encode();
  }
  return res;
}

bool GDns::SnoopDnsQuestions::decode(u_char* udpData, size_t dataLen, int count, size_t* offset)
{
  for (int i = 0; i < count; i++)
  {
    SnoopDnsQuestion question;
    if (!question.decode(udpData, dataLen, offset)) return false;
    push_back(question);
  }
  return true;
}

// ----------------------------------------------------------------------------
// SnoopDnsResourceRecord
// ----------------------------------------------------------------------------
QByteArray GDns::SnoopDnsResourceRecord::encode()
{
  QByteArray res;

  //res = GDns::encodeName(this->name);
  res.append((char)0xC0); // gilgil temp 2014.03.22
  res.append((char)0x0C);


  uint16_t _type = htons(this->type);
  res.append((const char*)&_type, sizeof(uint16_t));

  uint16_t _class_ = htons(this->class_);
  res.append((const char*)&_class_, sizeof(uint16_t));

  uint32_t _ttl = htonl(this->ttl);
  res.append((const char*)&_ttl, sizeof(uint32_t));

  uint16_t _dataLength = htons(this->dataLength);
  res.append((const char*)&_dataLength, sizeof(uint16_t));

  res += data;

  return res;
}

bool GDns::SnoopDnsResourceRecord::decode(u_char* udpData, size_t dataLen, size_t* offset)
{
  this->name = GDns::decodeName(udpData, dataLen, offset);
  if (this->name == "") return false;

  if (*offset + sizeof(uint16_t) > dataLen) return false;
  uint16_t* _type = (uint16_t*)(udpData + *offset);
  this->type = ntohs(*_type);
  *offset += sizeof(uint16_t);

  if (*offset  + sizeof(uint16_t) > dataLen) return false;
  uint16_t* _class_ = (uint16_t*)(udpData + *offset);
  this->class_ = ntohs(*_class_);
  *offset += sizeof(uint16_t);

  if (*offset  + sizeof(uint32_t) > dataLen) return false;
  uint32_t* _ttl = (uint32_t*)(udpData + *offset);
  this->ttl = ntohl(*_ttl);
  *offset += sizeof(uint32_t);

  if (*offset  + sizeof(uint16_t) > dataLen) return false;
  uint16_t* _dataLength = (uint16_t*)(udpData + *offset);
  this->dataLength= ntohs(*_dataLength);
  *offset += sizeof(uint16_t);

  if (*offset + this->dataLength > dataLen) return false;
  const char* data = (const char*)(udpData + *offset);
  this->data = QByteArray::fromRawData(data, this->dataLength);
  *offset += this->dataLength;

  return true;
}

// ----------------------------------------------------------------------------
// SnoopDnsResourceRecords
// ----------------------------------------------------------------------------
QByteArray GDns::SnoopDnsResourceRecords::encode()
{
  QByteArray res;
  foreach (SnoopDnsResourceRecord record, *this)
  {
    res += record.encode();
  }
  return res;
}

bool GDns::SnoopDnsResourceRecords::decode(u_char* udpData, size_t dataLen, int count, size_t* offset)
{
  for (int i = 0; i < count; i++)
  {
    SnoopDnsResourceRecord record;
    if (!record.decode(udpData, dataLen, offset)) return false;
    push_back(record);
  }
  return true;
}

// ----------------------------------------------------------------------------
// SnoopDns
// ----------------------------------------------------------------------------
QByteArray GDns::encode()
{
  QByteArray res;
  res.append((const char*)&dnsHdr, sizeof(DNS_HDR));

  // ----- gilgil temp 2014.03.22 -----
  /*
  uint16_t _id = htons(dnsHdr.id);
  res.append((const char*)&_id, sizeof(uint16_t));

  uint16_t _flags = htons(dnsHdr.flags);
  res.append((const char*)&_flags, sizeof(uint16_t));

  uint16_t _num_q = htons(dnsHdr.num_q);
  res.append((const char*)&_num_q, sizeof(uint16_t));

  uint16_t _num_answ_rr = htons(dnsHdr.num_answ_rr);
  res.append((const char*)&_num_answ_rr, sizeof(uint16_t));

  uint16_t _num_auth_rr = htons(dnsHdr.num_auth_rr);
  res.append((const char*)&_num_auth_rr, sizeof(uint16_t));

  uint16_t _num_addi_rr = htons(dnsHdr.num_addi_rr);
  res.append((const char*)&_num_addi_rr, sizeof(uint16_t));
  */
  // ----------------------------------

  res += questions.encode();
  res += answers.encode();
  res += authorities.encode();
  res += additionals.encode();

  return res;
}

bool GDns::decode(u_char* udpData, size_t dataLen, size_t* offset)
{
  if (*offset + sizeof(DNS_HDR) > dataLen) return false;
  memcpy(&this->dnsHdr, udpData, sizeof(DNS_HDR));
  *offset += sizeof(DNS_HDR);

  if (!questions.decode(udpData,   dataLen, ntohs(dnsHdr.num_q),       offset)) return false;
  if (!answers.decode(udpData,     dataLen, ntohs(dnsHdr.num_answ_rr), offset)) return false;
  if (!authorities.decode(udpData, dataLen, ntohs(dnsHdr.num_auth_rr), offset)) return false;
  if (!additionals.decode(udpData, dataLen, ntohs(dnsHdr.num_addi_rr), offset)) return false;

  return true;
}

QByteArray GDns::encodeName(QString name)
{
  QStringList labels = name.split('.');
  int count = labels.count();
  if (count == 0)
  {
    qWarning() << QString("label count is zero(%1)").arg(name);
    return "";
  }

  QByteArray res;
  for (int i = 0; i < count; i++)
  {
    QString label = labels.at(i);
    uint8_t size = label.size();
    res.append((const char*)&size, sizeof(uint8_t));
    res += label;
  }
  res.append((char)0x00);

  return res;
}

QString GDns::decodeName(u_char* udpData, size_t dataLen, size_t* offset)
{
  u_char* p = (u_char*)(udpData + *offset);
  QString res;
  bool first = true;
  while (true)
  {
    if (p - udpData > (int)dataLen) return false;
    uint8_t count = *p++;
    if (count == 0) break;

    if (count == 0xC0)
    {
      if (p - udpData > (int)dataLen) return false;
      size_t tempOffset = *p++;
      res = decodeName(udpData, dataLen, &tempOffset);
      *offset += 2;
      return res;
    }
    if (p - udpData + count > (int)dataLen) return false;
    QByteArray label((const char*)p, (int)count);
    p += count;

    if (first)
    {
      res += label;
      first = false;
    } else
    {
      res += ".";
      res += label;
    }
  }
  *offset = p - udpData;
  return res;
}
