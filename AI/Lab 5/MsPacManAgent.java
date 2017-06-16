package fs_student;

import game.controllers.GhostController;
import game.controllers.PacManController;
import game.core.G;
import game.core.Game;
import java.awt.Color;
import java.util.ArrayList;
import game.core.GameView;

public class MsPacManAgent implements PacManController//, Constants
{

	private static final int MIN_DISTANCE=20;	//if a ghost is this close, run away
	public int getAction(Game game, long time)
	{
		//Strategy 1:









		//Strategy 3: go after the pills and power pills
		int[] pills = game.getPillIndices();
		int[] powerPills = game.getPowerPillIndices();

		ArrayList<Integer> targets=new ArrayList<Integer>();

		for(int i = 0; i < pills.length; i++)			//check which pills are available
			if(game.checkPill(i))
				targets.add(pills[i]);

		for(int i = 0; i < powerPills.length; i++)	//check with power pills are available
			if(game.checkPowerPill(i))
				targets.add(powerPills[i]);




		//find the nearest edible ghost and go after them
		int minDist = 148;
		int minGhost = -1;

		for(int i = 0; i < 4; i++)
			if(game.getEdibleTime(i) > 0)
			{


				if( game.getPathDistance( game.getCurPacManLoc(), game.getCurGhostLoc(i) ) < minDist)
				{
					minDist = game.getPathDistance( game.getCurPacManLoc(), game.getCurGhostLoc(i) );
					minGhost = i;
				}
			}



		for(int i = 0; i < 4; i++)
		{
			if (game.getEdibleTime(i) == 0 && game.getLairTime(i) == 0)
			{
				if (game.getPathDistance(game.getCurPacManLoc(), game.getCurGhostLoc(i)) < 7)//7
					return game.getNextPacManDir(game.getCurGhostLoc(i), false, Game.DM.PATH);

				if (game.getPathDistance(game.getCurPacManLoc(), game.getCurGhostLoc(i) ) < 30) {
                    for (int j = 0; j < pills.length; j++) {//check which pills are available
                        if (game.checkPill(i))
                            targets.add(pills[i]);
                    }
                    int[] targetsArray = new int[targets.size()];		//convert from ArrayList to array

                    for(int j = 0; j < targetsArray.length; j++)
                        targetsArray[j] = targets.get(j);

                    return game.getNextPacManDir(game.getTarget(game.getCurPacManLoc(), targetsArray,true,Game.DM.PATH),true,Game.DM.PATH);

                }
			}

		}

		int[] targetsArray = new int[targets.size()];		//convert from ArrayList to array

		for(int i = 0; i < targetsArray.length; i++)
			targetsArray[i] = targets.get(i);

		if(minGhost != -1)	//we found an edible ghost
			return game.getNextPacManDir(game.getCurGhostLoc(minGhost),true, Game.DM.PATH);
		else
		//return the next direction once the closest target has been identified
		return game.getNextPacManDir(game.getTarget(game.getCurPacManLoc(), targetsArray,true,Game.DM.PATH),true,Game.DM.PATH);
	}
	}
