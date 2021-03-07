/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once


#define CSC_SLIST_HEAD_INIT(name) {&(name)}
#define CSC_SLIST_HEAD(name) struct csc_slist name = CSC_SLIST_HEAD_INIT(name)

struct csc_slist
{
	struct csc_slist * next;
};


void csc_slist_add (struct csc_slist * new, struct csc_slist * head)
{
	head->next = new;
}
