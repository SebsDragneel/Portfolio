using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.IO.Pipes;
using FullSailAFI.GamePlaying.CoreAI;

namespace FullSailAFI.GamePlaying
{
    public class StudentAI : Behavior
    {
        TreeVisLib treeVisLib;  // lib functions to communicate with TreeVisualization
        bool visualizationFlag = false;  // turn this on to use tree visualization (which you will have to implement via the TreeVisLib API)
                                         // WARNING: Will hang program if the TreeVisualization project is not loaded!

        public StudentAI()
        {
            if (visualizationFlag == true)
            {
                if (treeVisLib == null)  // should always be null, but just in case
                    treeVisLib = TreeVisLib.getTreeVisLib();  // WARNING: Creation of this object will hang if the TreeVisualization project is not loaded!
            }
        }

        //
        // This function starts the look ahead process to find the best move
        // for this player color.
        //
        public ComputerMove Run(int _nextColor, Board _board, int _lookAheadDepth)
        {
            ComputerMove nextMove = GetBestMove(_nextColor, _board, _lookAheadDepth);

            return nextMove;
        }

        //
        // This function uses look ahead to evaluate all valid moves for a
        // given player color and returns the best move it can find. This
        // method will only be called if there is at least one valid move
        // for the player of the designated color.
        //
        private ComputerMove GetBestMove(int color, Board board, int depth)
        {
            //TODO: the lab
            List<ComputerMove> Moves = new List<ComputerMove>();
            ComputerMove bestMove = null;
            Board newState = new Board();

            //  generate valid Moves for player;
            for (int r = 0; r < Board.Height; r++)
            {
                for (int c = 0; c < Board.Width; c++)
                {
                    if(board.IsValidMove(color, r, c) == true)
                    {
                        ComputerMove move = new ComputerMove(r, c);
                        Moves.Add(move);
                    }
                }
            }
                   
               foreach (ComputerMove move in Moves)
              {
                 newState.Copy(board);
                 newState.MakeMove(color, move.row, move.col);

                if (newState.IsTerminalState() ||  depth == 0)
                    move.rank = Evaluate(newState); /*ExampleAI.MinimaxAFI.EvaluateTest(newState);*/
                else
                {
                    if (newState.HasAnyValidMove(color * -1))
                        move.rank = GetBestMove(color * -1, newState, depth - 1).rank;
                    else
                        move.rank = GetBestMove(color, newState, depth - 1).rank;    
                }

                if(color == 1)
                {
                    if (bestMove == null || move.rank > bestMove.rank)
                        bestMove = move;
                }
                else
                {
                    if (bestMove == null || move.rank < bestMove.rank)
                        bestMove = move;
                }

                

            }

            return bestMove;

            throw new NotImplementedException();
        }

        #region Recommended Helper Functions

        private int Evaluate(Board _board)
        {
            int score = 0;
            int sum = 0;
            //TODO: determine score based on position of pieces
            for (int r = 0; r < Board.Height; r++)
            {
                for (int c = 0; c < Board.Width; c++)
                {
                    sum = _board.GetSquareContents(r, c);
                    if( (r == Board.Height - 1 || r == 0) && (c == Board.Width - 1 || c == 0))
                    {
                        sum *= 100;
                    }
                    else if ((r < Board.Height - 1 && r > 0) && (c == Board.Width - 1 || c == 0))
                    {
                        sum *= 10;
                    }
                    else if ((r == Board.Height - 1 || r == 0) && (c < Board.Width - 1 && c > 0))
                    {
                        sum *= 10;
                    }
                    else
                    {
                        sum *= 1;
                    }

                    score += sum;
                }
            }

            if(_board.IsTerminalState())
            {
                if(score > 0)
                {
                    score += 10000;
                }
                else
                    score -= 10000;
            }
            return score;
            
            throw new NotImplementedException();
        }

        #endregion

    }
}
