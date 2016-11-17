#pragma once

#include "helper.h"

void n_body_serial_v1(const Input &in)
{
	vector<Vector2d>
		force,
		position,
		velocity;
	vector<T>
		mass;
	T
		T_end,
		dt;
	int
		steps,
		N,
		q,
		tam;
	Vector2d
		p_diff;
	T
		distance,
		distance3;

	N = in.N;
	dt = in.dt;
	T_end = in.T;
	force.resize(N);
	position = in.bodiesInfo.position;
	velocity = in.bodiesInfo.velocity;
	mass = in.bodiesInfo.mass;

#ifdef DEBUG
	cout << "------------Initial Conditions:" << endl;
	print(position, velocity, force, mass);
#endif // DEBUG

	steps = (int)(T_end / dt);

	for (int i = 0; i < steps; i++)
	{
		//for each particle q, Compute total force on q
		for (int q = 0; q < N; q++)
		{
			for (int k = 0; k < q; k++)
			{
				p_diff = position[k] - position[q];
				distance = p_diff.norm();
				distance3 = distance * distance * distance;
				force[q] = force[q] - p_diff * ((G * mass[k] * mass[q]) / distance3);
			}

			for (int k = q + 1; k < N; k++)
			{
				p_diff = position[k] - position[q];
				distance = p_diff.norm();
				distance3 = distance * distance * distance;
				force[q] = force[q] - p_diff * ((G * mass[k] * mass[q]) / distance3);
			}
		}
		
		//for each particle q Compute position and velocity of q
		for (int q = 0; q < N; q++)
		{
			position[q] = position[q] + velocity[q] * dt;
			velocity[q] = velocity[q] + force[q] * (dt / mass[q]);
		}
	}

#ifdef DEBUG
	cout << "------------Resume:" << endl;
	cout << "Steps = " << steps << endl;
	print(position, velocity, force, mass);
#endif // DEBUG
}
void n_body_serial_v2(const Input &in)
{
	vector<Vector2d>
		force,
		position,
		velocity;
	vector<T>
		mass;
	T
		T_end,
		dt;
	int
		steps,
		N,
		q,
		tam;
	Vector2d
		f,
		p_diff;
	T
		distance,
		distance3;

	N = in.N;
	dt = in.dt;
	T_end = in.T;
	force.resize(N);
	position = in.bodiesInfo.position;
	velocity = in.bodiesInfo.velocity;
	mass = in.bodiesInfo.mass;

#ifdef DEBUG
	cout << "------------Initial Conditions:" << endl;
	print(position, velocity, force, mass);
#endif // DEBUG

	steps = (int)(T_end / dt);

	for (int i = 0; i < steps; i++)
	{
		//for each particle q, Compute total force on q
		for (int q = 0; q < N; q++)
		{
			for (int k = q + 1; k < N; k++)
			{
				p_diff = position[q] - position[k];
				distance = p_diff.norm();
				distance3 = distance * distance * distance;
		
				f = p_diff * ((G * mass[q] * mass[k]) / distance3);
				force[q] = force[q] + f;
				force[k] = force[k] - f;
			}
		}
		
		//for each particle q Compute position and velocity of q
		for (int q = 0; q < N; q++)
		{
			position[q] = position[q] + velocity[q] * dt;
			velocity[q] = velocity[q] + force[q] * (dt / mass[q]);
		}
	}

#ifdef DEBUG
	cout << "------------Resume:" << endl;
	cout << "Steps = " << steps << endl;
	print(position, velocity, force, mass);
#endif // DEBUG
}
void n_body_omp_v1(const Input &in)
{
	vector<Vector2d>
		force,
		position,
		velocity;
	vector<T>
		mass;
	T
		T_end,
		dt;
	int
		threads,
		steps,
		N,
		q,
		tam;
	Vector2d
		p_diff;
	T
		distance,
		distance3;

	N = in.N;
	dt = in.dt;
	T_end = in.T;
	force.resize(N);
	position = in.bodiesInfo.position;
	velocity = in.bodiesInfo.velocity;
	mass = in.bodiesInfo.mass;

#ifdef DEBUG
	cout << "------------Initial Conditions:" << endl;
	print(position, velocity, force, mass);
#endif // DEBUG

	steps = (int)(T_end / dt);
	#pragma omp parallel
	{
		threads = omp_get_num_threads();

		for (int i = 0; i < steps; i++)
		{
			//for each particle q, Compute total force on q
			#pragma omp parallel for schedule(static, N/threads) private(p_diff, distance, distance3)
			for (int q = 0; q < N; q++)
			{
				for (int k = 0; k < q; k++)
				{
					p_diff = position[k] - position[q];
					distance = p_diff.norm();
					distance3 = distance * distance * distance;
					force[q] = force[q] - p_diff * ((G * mass[k] * mass[q]) / distance3);
				}

				for (int k = q + 1; k < N; k++)
				{
					p_diff = position[k] - position[q];
					distance = p_diff.norm();
					distance3 = distance * distance * distance;
					force[q] = force[q] - p_diff * ((G * mass[k] * mass[q]) / distance3);
				}
			}
		
			//for each particle q Compute position and velocity of q
			#pragma omp parallel for schedule(static, N/threads)
			for (int q = 0; q < N; q++)
			{
				position[q] = position[q] + velocity[q] * dt;
				velocity[q] = velocity[q] + force[q] * (dt / mass[q]);
			}
		}
	}

#ifdef DEBUG
	cout << "------------Resume:" << endl;
	cout << "Steps = " << steps << endl;
	print(position, velocity, force, mass);
#endif // DEBUG
}
void n_body_omp_v2(const Input &in)
{
	vector<Vector2d>
		force,
		position,
		velocity;
	vector<T>
		mass;
	T
		T_end,
		dt;
	int
		threads,
		steps,
		N,
		q,
		tam;
	Vector2d
		f,
		p_diff;
	T
		distance,
		distance3;

	N = in.N;
	dt = in.dt;
	T_end = in.T;
	force.resize(N);
	position = in.bodiesInfo.position;
	velocity = in.bodiesInfo.velocity;
	mass = in.bodiesInfo.mass;

	vector<omp_lock_t> my_locks;
	my_locks.resize(N);
	for (int i = 0; i < N; i++)
		omp_init_lock(&(my_locks[i]));


#ifdef DEBUG
	cout << "------------Initial Conditions:" << endl;
	print(position, velocity, force, mass);
#endif // DEBUG

	steps = (int)(T_end / dt);

	#pragma omp parallel
	{
		threads = omp_get_num_threads();

		for (int i = 0; i < steps; i++)
		{
			//for each particle q, Compute total force on q
			#pragma omp parallel for schedule(static, N/threads) private(p_diff, distance, distance3, f)
			for (int q = 0; q < N; q++)
			{
				for (int k = q + 1; k < N; k++)
				{
					p_diff = position[q] - position[k];
					distance = p_diff.norm();
					distance3 = distance * distance * distance;
		
					f = p_diff * ((G * mass[q] * mass[k]) / distance3);
				
					omp_set_lock(&(my_locks[q]));
					force[q] = force[q] + f;
					omp_unset_lock(&(my_locks[q]));

					omp_set_lock(&(my_locks[k]));
					force[k] = force[k] - f;
					omp_unset_lock(&(my_locks[k]));
				}
			}
		
			//for each particle q Compute position and velocity of q
			#pragma omp parallel for
			for (int q = 0; q < N; q++)
			{
				position[q] = position[q] + velocity[q] * dt;
				velocity[q] = velocity[q] + force[q] * (dt / mass[q]);
			}
		}
	}

#ifdef DEBUG
	cout << "------------Resume:" << endl;
	cout << "Steps = " << steps << endl;
	print(position, velocity, force, mass);
#endif // DEBUG
}
void n_body_omp_v3(const Input &in)
{
	vector<Vector2d>
		*loc_forces,
		force,
		position,
		velocity;
	vector<T>
		mass;
	T
		T_end,
		dt;
	int
		thread_id,
		threads,
		steps,
		N,
		q,
		tam;
	Vector2d
		f,
		p_diff;
	T
		distance,
		distance3;

	N = in.N;
	dt = in.dt;
	T_end = in.T;
	force.resize(N);
	position = in.bodiesInfo.position;
	velocity = in.bodiesInfo.velocity;
	mass = in.bodiesInfo.mass;

	vector<omp_lock_t> my_locks;
	my_locks.resize(N);
	for (int i = 0; i < N; i++)
		omp_init_lock(&(my_locks[i]));


#ifdef DEBUG
	cout << "------------Initial Conditions:" << endl;
	print(position, velocity, force, mass);
#endif // DEBUG

	steps = (int)(T_end / dt);

	#pragma omp parallel private(thread_id)
	{
		#pragma omp master
		{
			threads = omp_get_num_threads();
			loc_forces = new vector<Vector2d>[threads];
			for (int i = 0; i < threads; i++)
				loc_forces[i].resize(N);
		}

		thread_id = omp_get_thread_num();

		#pragma omp barrier

		for (int i = 0; i < steps; i++)
		{
			//for each particle q, Compute total force on q
			//remove static schedule 
			#pragma omp parallel for schedule(static, N/threads) private(p_diff, distance, distance3, f)
			for (int q = 0; q < N; q++)
			{
				for (int k = q + 1; k < N; k++)
				{
					p_diff = position[q] - position[k];
					distance = p_diff.norm();
					distance3 = distance * distance * distance;
		
					f = p_diff * ((G * mass[q] * mass[k]) / distance3);
				
					//omp_set_lock(&(my_locks[q]));
					//force[q] = force[q] + f;
					//omp_unset_lock(&(my_locks[q]));

					//omp_set_lock(&(my_locks[k]));
					//force[k] = force[k] - f;
					//omp_unset_lock(&(my_locks[k]));
					loc_forces[thread_id][q] = loc_forces[thread_id][q] + f;
					loc_forces[thread_id][k] = loc_forces[thread_id][q] - f;
				}
			}

			#pragma omp parallel for
			for (int q = 0; q < N; q++)
			{
				for (int id = 0; id < threads; id++)
				{
					force[q] = force[q] + loc_forces[thread_id][q];
				}
			}
			
		
			//for each particle q Compute position and velocity of q
			#pragma omp parallel for
			for (int q = 0; q < N; q++)
			{
				position[q] = position[q] + velocity[q] * dt;
				velocity[q] = velocity[q] + force[q] * (dt / mass[q]);
			}
		}
	}

#ifdef DEBUG
	cout << "------------Resume:" << endl;
	cout << "Steps = " << steps << endl;
	print(position, velocity, force, mass);
#endif // DEBUG
}
void n_body_mpi_v1(const MPI_Input &in)
{
	vector<T>
		l_force,
		mass,
		position,
		g_velocity,
		l_velocity;
	T
		T_end,
		dt,
		p_diff[2],
		distance,
		distance3;
	int
		steps,
		l_N,
		g_N,
		q,
		tam;
	double
		in_buf[3];

	if (id == 0)
	{
		in_buf[0] = in.N;
		in_buf[1] = in.dt;
		in_buf[2] = in.T;

		mass = in.mpi_bodiesInfo.mass;
		position = in.mpi_bodiesInfo.position;

		g_velocity = in.mpi_bodiesInfo.velocity;
	}

	MPI_Bcast(&in_buf[0], 3, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	g_N = (int) (in_buf[0]);
	dt = in_buf[1];
	T_end = in_buf[2];

	mass.resize(g_N);
	position.resize(2 * g_N);

	l_N = g_N / nTasks;

	if (id == 0)
	{
		cout << "g_N = " << g_N << endl;
		cout << "l_N = " << l_N << endl;
		cout << "dt = " << dt << endl;
		cout << "T_end = " << T_end << endl;
		cout << "mass.size() = " << mass.size() << endl;
		cout << "position.size() = " << position.size() << endl;
	}

	MPI_Bcast(	&mass[0], 
				g_N, 
				MPI_DOUBLE, 
				0, 
				MPI_COMM_WORLD);
	MPI_Bcast(	&position[0], 
				2 * g_N, 
				MPI_DOUBLE, 
				0, 
				MPI_COMM_WORLD);

	

	l_velocity.resize(2 * l_N);
	MPI_Scatter(&g_velocity[0],
				2 * l_N,
				MPI_DOUBLE,
				&l_velocity[0],
				2 * l_N,
				MPI_DOUBLE,
				0,
				MPI_COMM_WORLD);

	l_force.resize(2 * l_N);

#ifdef DEBUG
	cout << "------------Initial Conditions:" << endl;
	//print(position, velocity, force, mass);
#endif // DEBUG

	steps = (int)(T_end / dt);

	for (int i = 0; i < steps; i++)
	{
		//for each particle q, Compute total force on q
		for (int l_q = id*l_N; l_q < (id + 1)*l_N; l_q++)
		{
			for (int k = 0; k < l_q; k++)
			{
				p_diff[0] = position[2*k] - position[2*l_q];
				p_diff[1] = position[2*k+1] - position[2*l_q+1];
				distance = sqrt(p_diff[0]*p_diff[0] + p_diff[1]*p_diff[1]);
				distance3 = distance * distance * distance;
				l_force[2*(l_q % l_N)] -= p_diff[0] * ((G * mass[k] * mass[l_q]) / distance3);
				l_force[2*(l_q % l_N) + 1] -= p_diff[1] * ((G * mass[k] * mass[l_q]) / distance3);
			}

			for (int k = l_q + 1; k < g_N; k++)
			{
				p_diff[0] = position[2*k] - position[2*l_q];
				p_diff[1] = position[2*k+1] - position[2*l_q+1];
				distance = sqrt(p_diff[0]*p_diff[0] + p_diff[1]*p_diff[1]);
				distance3 = distance * distance * distance;
				l_force[2*(l_q % l_N)] -= p_diff[0] * ((G * mass[k] * mass[l_q]) / distance3);
				l_force[2*(l_q % l_N) + 1] -= p_diff[1] * ((G * mass[k] * mass[l_q]) / distance3);
			}
		}
		
		//for each particle q Compute position and velocity of q
		for (int l_q = id*l_N; l_q < (id + 1)*l_N; l_q++)
		{
			position[2*l_q] += l_velocity[2*(l_q % l_N)] * dt;
			position[2*l_q] += l_velocity[2*(l_q % l_N) + 1] * dt;

			l_velocity[2*(l_q % l_N)] += l_force[2*(l_q % l_N)] * (dt / mass[l_q]);
			l_velocity[2*(l_q % l_N) + 1] += l_force[2*(l_q % l_N) + 1] * (dt / mass[l_q]);
		}

		MPI_Allgather(	&(position[2*id*l_N]),
						2 * l_N, 
						MPI_DOUBLE, 
						&position[0], 
						2 * l_N, 
						MPI_DOUBLE, 
						MPI_COMM_WORLD); 
	}

	MPI_Gather(	&(l_velocity[2*id*l_N]),
					2 * l_N, 
					MPI_DOUBLE, 
					&g_velocity[0], 
					2 * l_N, 
					MPI_DOUBLE,
					0,
					MPI_COMM_WORLD); 

	if (id == 0)
	{
#ifdef DEBUG
		cout << "------------Resume:" << endl;
		cout << "Steps = " << steps << endl;
		//MPI_print(position, g_velocity);
#endif // DEBUG		
	}


}
void n_body_mpi_v2(const MPI_Input &in)
{
	cout << "Sorry, I didn't have enough time!" << endl;
}
