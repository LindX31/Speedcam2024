# -*- coding: utf-8 -*-
"""
Created on Tue Nov 21 18:41:07 2023

@author: Jade G
"""

import numpy as np

## fonctions réalisant les opérations élémentaires sur les lignes de T
def echange(T,i,j):
    """ permet de échanger les lignes i et j du tableau T"""
    n=len(T[0])
    for k in range(n):
        T[i][k],T[j][k]=T[j][k],T[i][k]

def ajoutligne(T,i,j,a):
    """ permet de faire Li <--- Li + aLj, sur T"""
    n=len(T[0])
    for k in range(n):
        T[i][k]+= a*T[j][k]
    return T

def multiligne(T,i,a):
    """ permet de faire Li <--- aLi, sur T"""
    n=len(T[0])
    for k in range(n):
        T[i][k]= a*T[i][k]

## sous fonction crées initiallement chargée chacune d'une étape de l'alogrithme du pivot de Gauss
def recherche_pivot(T,B, j):
    """ on recherche un pivot dans la colonne j en dessous de la ligne j : on recherche celui max en module pour éviter les erreurs d'approx"""
    n=len(T)
    imax=j
    max=0
    for i in range(j,n):
        if abs(T[i][j])>max:
            imax=i
            max=abs(T[imax][j])
    if imax!=j:
        echange(T,imax,j)
        echange(B,imax,j)
        return True
    return False


def descente_pivot(T,B,n):
    for j in range(n):
        recherche_pivot(T,B,j)
        for i in range(j+1,n):
            aij=T[i][j]
            ajj=T[j][j]
            if ajj!=0:
                ajoutligne(T,i,j, -aij/ajj)
                ajoutligne(B,i,j, -aij/ajj)
    return T

def remontée_pivot(T,B,n):
    for j in range(n-1, -1, -1):
        ajj=T[j][j]
        if ajj!=0:
            multiligne(T,j, 1/ajj)
            multiligne(B,j, 1/ajj)
            for k in range(j-1, -1, -1):
                akj,ajj=T[k][j],T[j,j]
                ajoutligne(T,k,j, -akj/ajj)
                ajoutligne(B,k,j, -akj/ajj)
    return T


## fonction qui marche, complexité temporelle : O(n3), complexité spatiale : négligeable
def calcul_solution(T,B):
    """T un tableau carré, inversible, de taille n, et B colonne à n lignes
    renvoie l'unique solution du sytème"""
    n=len(T)
    for j in range(n):
        n=len(T)
        kmax=j
        for k in range(j,n):
            if abs(T[k][j])>abs(T[kmax][j]): ## on recherche le pivot le plus en valeur absolue : celui qui occasionnera le moins d'erreur d'arrondi
                kmax=k
        if kmax!=j:
            echange(T,kmax,j)
            echange(B,kmax,j)
        ajj=T[j][j]
        if ajj!=0:
            for i in range(j+1,n):
                aij=T[i][j]
                ajoutligne(T,i,j, -aij/ajj)
                ajoutligne(B,i,j, -aij/ajj)
    for j in range(n-1, -1, -1):
        ajj=T[j][j]
        if ajj!=0:
            multiligne(T,j, 1/ajj)
            multiligne(B,j, 1/ajj)
            for k in range(j-1, -1, -1):
                akj,ajj=T[k][j],T[j,j]
                ajoutligne(T,k,j, -akj/ajj)
                ajoutligne(B,k,j, -akj/ajj)
    return B




## les tableaux doivent être des flottants

def dpparabole(X,Y):
    """permet de construire le tableau T,B pour ensuite utiliser calcul solution pour la résolution"""
    n=len(X)
    T=np.zeros((3,3), dtype=float)
    B=np.zeros((3,1), dtype=float)
    for k in range(3):
        for i in range(n):
            B[k]+=Y[i]*X[i]**(2-k)## l'incrémentation de B doit être ici on le fait 9 fois
            for j in range(3):
                T[k,j]+=X[i]**(4-k-j)
    return T,B



def déterminationparabole(X,Y):
    T,B=dpparabole(X,Y)
    return calcul_solution(T,B)


# ---------------------------------------------
# ---------------------------------------------

# moyenne et pondération binomiale


from math import sqrt

def moyenne(L):
    n=len(L)
    m=0
    for i in range(n):
        m+=L[i]
    return m/n


#ponderebino2 est plus efficace car il permet de donner plus d'importance aux dernière données


def ponderebino2(X):
    s=0
    sigma=0
    for i in range(len(X)):
        b=bino(i,2*len(X)-1)
        s+=X[i]*b
        sigma+=b
    return s/sigma
n=int(input("nombre"))

def bino(i,n):
    return fact(n)/fact(n-i)/fact(i)

def fact(i):
    if i<=1:
        return 1
    else :
        return fact(i-1)*i




