using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FullSailAFI.SteeringBehaviors.Core;

namespace FullSailAFI.SteeringBehaviors.StudentAI
{
    public class Flock
    {
        public float AlignmentStrength { get; set; }
        public float CohesionStrength { get; set; }
        public float SeparationStrength { get; set; }
        public List<MovingObject> Boids { get; protected set; }
        public Vector3 AveragePosition { get; set; }
        protected Vector3 AverageForward { get; set; }
        public float FlockRadius { get; set; }

        #region Constructors
        public Flock()
        {
        }
        #endregion

        #region TODO Suggested helper methods

        private void CalculateAverages()
        {
            Vector3 avgForward = Vector3.Empty;
            Vector3 avgPosition = Vector3.Empty;

            for (int i = 0; i < Boids.Count; i++)
            {
                avgPosition += Boids[i].Position;
                avgForward += Boids[i].Velocity;
            }
            avgForward = avgForward / Boids.Count;
            avgPosition = avgPosition / Boids.Count;
            AverageForward = avgForward;
            AveragePosition = avgPosition;
        }

        private Vector3 CalculateAlignmentAcceleration(MovingObject boid)
        {
           Vector3 vector = AverageForward / boid.MaxSpeed;
            if (vector.Length > 1)
            {
                vector.Normalize();
            }
        
            return vector * AlignmentStrength;
        }

        private Vector3 CalculateCohesionAcceleration(MovingObject boid)
        {
            Vector3 vector = AveragePosition - boid.Position;
            float distance = vector.Length;
            vector.Normalize();

            if (distance < FlockRadius)
                vector *= distance / FlockRadius;

            return vector * CohesionStrength;

        }

        private Vector3 CalculateSeparationAcceleration(MovingObject boid)
        {
          Vector3  sum = Vector3.Empty;

            foreach (MovingObject otherBoid in Boids)
            {
               Vector3 vector = boid.Position - otherBoid.Position;
                float distance = vector.Length;
               float safeDistance = boid.SafeRadius + otherBoid.SafeRadius;

                if (distance < safeDistance)
                {
                    vector.Normalize();
                    vector *= (safeDistance - distance) / safeDistance;
                    sum += vector;
                }
            }

            if (sum.Length > 1.0f)
                sum.Normalize();

            return sum *  SeparationStrength;

        }

        #endregion

        #region TODO

        public virtual void Update(float deltaTime)
        {
           CalculateAverages();

            foreach (MovingObject Boid in Boids)
            {
                Vector3 Accel;
                Accel = CalculateAlignmentAcceleration(Boid);
                Accel += CalculateCohesionAcceleration(Boid);
                Accel += CalculateSeparationAcceleration(Boid);
                Accel *= Boid.MaxSpeed * deltaTime;

                Boid.Velocity += Accel;

                if (Boid.Velocity.Length > Boid.MaxSpeed)
                {
                    Boid.Velocity.Normalize();
                    Boid.Velocity *= Boid.MaxSpeed;
                }
                Boid.Update(deltaTime);
            }
            return;
        }
        #endregion
    }
}
