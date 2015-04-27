/*
 * Copyright 2004 Free Software Foundation, Inc.
 * Copyright 2014 Xavier Mendez <me@jmendeth.com>
 *
 * This file is part of RDS Utils
 *
 * RDS Utils is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * RDS Utils is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RDS Utils; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef RDS_UTILS_ENCODER_RDS_H
#define RDS_UTILS_ENCODER_RDS_H

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>

namespace RDS_Utils {


class EncoderRDS {
private:
	unsigned int infoword[4];
	unsigned int checkword[4];
	unsigned int block[4];

protected:
	void processNodes(xmlNode*);
	void processField(const char*, const char*, const int);
	void countGroups();
	void createGroup(const int, const bool);
	void prepareGroup0(const bool);
	void prepareGroup2(const bool);
	void prepareGroup4A();
	void prepareGroup8A();
	void prepareBuffer(int);
	unsigned int encodeAF(double);
	unsigned int calcSyndrome(unsigned long, unsigned char);

public:
	unsigned char **buffer;
	// FIXME make this a struct (or a class)
	unsigned int PI;
	bool TP;
	unsigned char PTY;
	bool TA;
	bool MuSp;
	bool MS;
	bool AH;
	bool compressed;
	bool static_pty;
	double AF1;
	double AF2;
	char PS[8];
	unsigned char radiotext[64];
	int DP;
	int extent;
	int event;
	int location;

	/* each type 0 group contains 2 out of 8 PS characters;
	 * this is used to count 0..3 and send all PS characters */
	int d_g0_counter;
	/* each type 2A group contains 4 out of 64 RadioText characters;
	 * each type 2B group contains 2 out of 32 RadioText characters;
	 * this is used to count 0..15 and send all RadioText characters */
	int d_g2_counter;
	/* points to the current buffer being prepared/streamed
	 * used in create_group() and in work() */
	int d_current_buffer;
	/* loops through the buffer, pushing out the symbols */
	int d_buffer_bit_counter;
	/* 0..16+A/B = 32 groups. 0A is groups[0], 0B is groups[16]. use %16.
	 * ==0 means group not present, ==1 means group present */
	int ngroups;
	int groups[32];
	/* nbuffers might be != ngroups, e.g. group 0A needs 4 buffers */
	int nbuffers;

	EncoderRDS();
	~EncoderRDS();

	void reset();
	void encode();
	int readXML(const char*);
};


}

#endif /* RDS_UTILS_ENCODER_RDS_H */
