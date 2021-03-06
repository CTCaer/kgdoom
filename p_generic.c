// generic floor / ceiling movement
// by kgsws
#include "z_zone.h"
#include "doomdef.h"
#include "p_local.h"
#include "s_sound.h"
#include "doomstat.h"
#include "r_state.h"

#include "p_generic.h"

//
// Ceiling
//

void T_GenericCeiling(generic_plane_t *gp)
{
	fixed_t speed;
	sector_t *sec = gp->info.sector;

	if(gp->info.startz < gp->info.stopz)
	{
		// raise
		if(sec->ceilingheight + gp->speed > gp->info.stopz)
			speed = gp->info.stopz - sec->ceilingheight;
		else
			speed = gp->speed;
		sec->ceilingheight += speed;
		P_ChangeSector(sec, !!gp->info.crushspeed);
	} else
	if(gp->info.startz > gp->info.stopz)
	{
		// lower
		if(sec->ceilingheight - gp->speed < gp->info.stopz)
			speed = sec->ceilingheight - gp->info.stopz;
		else
			speed = gp->speed;
		sec->ceilingheight -= speed;
		if(P_ChangeSector(sec, !!gp->info.crushspeed))
		{
			// move blocked
			if(gp->info.crushspeed)
			{
				// change speed and continue;
				gp->speed = gp->info.crushspeed;
			} else
			{
				// return back
				sec->ceilingheight += speed;
				P_ChangeSector(sec, false);
			}
		}
	}
	// finished ?
	if(sec->ceilingheight == gp->info.stopz)
	{
		if(gp->info.stopsound)
			S_StartSound((mobj_t *)&sec->soundorg, gp->info.stopsound, SOUND_BODY);
		sec->specialdata = NULL;
		sec->ceilingpic = gp->info.stoppic;
		P_RemoveThinker(&gp->thinker);
	} else
	{
	    if(!(leveltime&7) && gp->info.movesound)
		S_StartSound((mobj_t *)&sec->soundorg, gp->info.movesound, SOUND_BODY);
	}
}

void P_GenericSectorCeiling(sector_t *sec, generic_info_t *info)
{
	generic_plane_t *gp;

	// remove old one
	if(sec->specialdata)
		P_RemoveThinker(sec->specialdata);
	// check speed
	if(!info->speed)
		return;
	// add new one
	gp = Z_Malloc(sizeof(generic_plane_t), PU_LEVSPEC, 0);
	P_AddThinker(&gp->thinker);
	gp->info = *info;
	gp->thinker.function.acp1 = (actionf_p1)T_GenericCeiling;
	sec->specialdata = gp;
	// set starting parameters
	gp->speed = gp->info.speed;
	sec->ceilingheight = info->startz;
	sec->ceilingpic = info->startpic;
	if(info->startsound)
		S_StartSound((mobj_t *)&sec->soundorg, info->startsound, SOUND_BODY);
}

//
// Floor
//

void T_GenericFloor(generic_plane_t *gp)
{
	fixed_t speed;
	sector_t *sec = gp->info.sector;

	if(gp->info.startz > gp->info.stopz)
	{
		// lower
		if(sec->floorheight - gp->speed < gp->info.stopz)
			speed = sec->floorheight - gp->info.stopz;
		else
			speed = gp->speed;
		sec->floorheight -= speed;
		P_ChangeSector(sec, !!gp->info.crushspeed);
	} else
	if(gp->info.startz < gp->info.stopz)
	{
		// raise
		if(sec->floorheight + gp->speed > gp->info.stopz)
			speed = gp->info.stopz - sec->floorheight;
		else
			speed = gp->speed;
		sec->floorheight += speed;
		if(P_ChangeSector(sec, !!gp->info.crushspeed))
		{
			// move blocked
			if(gp->info.crushspeed)
			{
				// change speed and continue;
				gp->speed = gp->info.crushspeed;
			} else
			{
				// return back
				sec->floorheight -= speed;
				P_ChangeSector(sec, false);
			}
		}
	}
	// finished ?
	if(sec->floorheight == gp->info.stopz)
	{
		if(gp->info.stopsound)
			S_StartSound((mobj_t *)&sec->soundorg, gp->info.stopsound, SOUND_BODY);
		sec->specialdata = NULL;
		sec->floorpic = gp->info.stoppic;
		P_RemoveThinker(&gp->thinker);
	} else
	{
	    if(!(leveltime&7) && gp->info.movesound)
		S_StartSound((mobj_t *)&sec->soundorg, gp->info.movesound, SOUND_BODY);
	}
}

void P_GenericSectorFloor(sector_t *sec, generic_info_t *info)
{
	generic_plane_t *gp;

	// remove old one
	if(sec->specialdata)
		P_RemoveThinker(sec->specialdata);
	// check speed
	if(!info->speed)
		return;
	// add new one
	gp = Z_Malloc(sizeof(generic_plane_t), PU_LEVSPEC, 0);
	P_AddThinker(&gp->thinker);
	gp->info = *info;
	gp->thinker.function.acp1 = (actionf_p1)T_GenericFloor;
	sec->specialdata = gp;
	// set starting parameters
	gp->speed = gp->info.speed;
	sec->floorheight = info->startz;
	sec->floorpic = info->startpic;
	if(info->startsound)
		S_StartSound((mobj_t *)&sec->soundorg, info->startsound, SOUND_BODY);
}

